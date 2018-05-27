#pragma once

#include <string>

class ProfilePictureManager {

private:
    struct CacheInfo {
        std::string url;
        std::string etag;
    };

    std::string profile_pictures_dir;

    std::string get_cache_image(std::string const& cid);

    std::string get_cache_meta(std::string const& cid);

    CacheInfo read_cache_info(std::string const& file);

    void write_cache_info(std::string const& file, CacheInfo const& info);

public:
    static ProfilePictureManager instance;

    ProfilePictureManager();

    void get_image_async(std::string const& cid, std::string const& url,
                         std::function<void (Glib::RefPtr<Gdk::Pixbuf>)> cb);

    void get_image(std::string const& cid, std::string const& url, std::function<void (Glib::RefPtr<Gdk::Pixbuf>)> cb);

    struct DownloadImageInfo {
        std::string etag;

        Glib::RefPtr<Gdk::Pixbuf> image;
        bool image_cached;
    };

    DownloadImageInfo download_image(std::string const& url, std::string const& etag = std::string());

};