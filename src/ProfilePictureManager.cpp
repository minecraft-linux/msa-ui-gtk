#include <curl/curl.h>
#include <stdexcept>
#include <cstring>
#include <gdkmm.h>
#include <thread>
#include <giomm.h>
#include "ProfilePictureManager.h"

ProfilePictureManager ProfilePictureManager::instance;

ProfilePictureManager::ProfilePictureManager() {
    profile_pictures_dir = Glib::get_user_cache_dir() + "/msa/profile_pictures/";
    g_mkdir_with_parents(profile_pictures_dir.c_str(), 0755);
}

std::string ProfilePictureManager::get_cache_image(std::string const& cid) {
    return profile_pictures_dir + cid + ".png";
}

std::string ProfilePictureManager::get_cache_meta(std::string const& cid) {
    return profile_pictures_dir + cid + ".png.meta";
}

ProfilePictureManager::CacheInfo ProfilePictureManager::read_cache_info(std::string const& file) {
    CacheInfo ret;
    Glib::KeyFile key_file;
    if (!key_file.load_from_file(file))
        return ret;
    ret.url = key_file.get_string("CacheInfo", "URL");
    ret.etag = key_file.get_string("CacheInfo", "ETag");
    return ret;
}

void ProfilePictureManager::write_cache_info(std::string const& file, CacheInfo const& info) {
    Glib::KeyFile key_file;
    key_file.set_string("CacheInfo", "URL", info.url);
    key_file.set_string("CacheInfo", "ETag", info.etag);
    if (!key_file.save_to_file(file))
        throw std::runtime_error("Failed to write cache meta info");
}

void ProfilePictureManager::get_image_async(std::string const& cid, std::string const& url,
                                            std::function<void(Glib::RefPtr<Gdk::Pixbuf>)> cb) {
    std::thread t([this, cid, url, cb]() {
        get_image(cid, url, cb);
    });
    t.detach();
}

void ProfilePictureManager::get_image(std::string const& cid, std::string const& url,
                                      std::function<void(Glib::RefPtr<Gdk::Pixbuf>)> cb) {
    auto meta_file = get_cache_meta(cid);
    auto image_file = get_cache_image(cid);
    std::string etag;
    if (Glib::file_test(meta_file, Glib::FILE_TEST_EXISTS) && Glib::file_test(image_file, Glib::FILE_TEST_EXISTS)) {
        try {
            auto meta = read_cache_info(meta_file);
            if (meta.url == url) {
                etag = meta.etag;

                cb(Gdk::Pixbuf::create_from_file(image_file));
            }
        } catch (Glib::Exception& e) {
            printf("Failed to load image from cache (%s): %s\n", image_file.c_str(), e.what());
        }
    }
    ProfilePictureManager::DownloadImageInfo img;
    try {
        img = download_image(url, etag);
        if (img.image_cached) {
            printf("Image in cache up-to-date\n");
            return;
        }
    } catch (std::exception& e) {
        printf("Failed to download profile image (%s): %s\n", url.c_str(), e.what());
        return;
    }
    if (img.image) {
        cb(img.image);
        try {
            write_cache_info(meta_file, {url, img.etag});
            img.image->save(image_file, "png");
        } catch (Glib::Exception& e) {
            printf("Failed to write cache info: %s\n", image_file.c_str());
            remove(meta_file.c_str());
            remove(image_file.c_str());
        }
    } else {
        remove(meta_file.c_str());
        remove(image_file.c_str());
    }
}

ProfilePictureManager::DownloadImageInfo ProfilePictureManager::download_image(std::string const& url,
                                                                               std::string const& etag) {
    CURL* curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("Failed to init CURL");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    DownloadImageInfo ret;
    Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
    loader->set_size(32, 32);

    struct curl_slist* headers = nullptr;
    std::string ifNoneMatchHeader;
    if (!etag.empty()) {
        ifNoneMatchHeader = "If-None-Match: \"" + etag + "\"";;
        headers = curl_slist_append(headers, ifNoneMatchHeader.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, (size_t (*)(char*, size_t, size_t, DownloadImageInfo*)) [](char* buffer, size_t size, size_t nitems, DownloadImageInfo* data) {
        if (size * nitems > 6 && memcmp(buffer, "ETag: ", 6) == 0) {
            data->etag = std::string(&buffer[6], nitems * size - 6 - 2);
        }
        return nitems * size;
    });
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &ret);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, (size_t (*)(void*, size_t, size_t, Gdk::PixbufLoader*)) [](void* ptr, size_t size, size_t nmemb, Gdk::PixbufLoader* loader) {
        loader->write((guint8*) ptr, size * nmemb);
        return size * nmemb;
    });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, loader.get());

    CURLcode res = curl_easy_perform(curl);
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK)
        throw std::runtime_error("curl_easy_perform failed");

    try {
        loader->close();
    } catch (Glib::Exception& e) {
    }
    if (response_code == 200)
        ret.image = loader->get_pixbuf();
    else if (response_code == 304)
        ret.image_cached = true;
    return ret;
}