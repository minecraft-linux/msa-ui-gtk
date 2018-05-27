#include <curl/curl.h>
#include <stdexcept>
#include <cstring>
#include <gdkmm.h>
#include "ProfilePictureManager.h"

ProfilePictureManager ProfilePictureManager::instance;

Glib::RefPtr<Gdk::Pixbuf> ProfilePictureManager::downloadImage(std::string const& cid, std::string const& url,
                                                               std::string const& etag) {
    CURL* curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("Failed to init CURL");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    struct HeaderData {
        std::string etag;
    } headerData;
    Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
    loader->set_size(32, 32);

    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, (size_t (*)(char*, size_t, size_t, HeaderData*)) [](char* buffer, size_t size, size_t nitems, HeaderData* data) {
        if (size * nitems > 6 && memcmp(buffer, "ETag: ", 6) == 0)
            data->etag = std::string(&buffer[6], nitems * size - 6);
        return nitems * size;
    });
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headerData);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, (size_t (*)(void*, size_t, size_t, Gdk::PixbufLoader*)) [](void* ptr, size_t size, size_t nmemb, Gdk::PixbufLoader* loader) {
        loader->write((guint8*) ptr, size * nmemb);
        return size * nmemb;
    });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, loader.get());

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK)
        throw std::runtime_error("curl_easy_perform failed");

    loader->close();
    return loader->get_pixbuf();
}