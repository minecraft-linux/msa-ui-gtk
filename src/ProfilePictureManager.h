#pragma once

#include <string>

class ProfilePictureManager {

public:
    static ProfilePictureManager instance;

    Glib::RefPtr<Gdk::Pixbuf> downloadImage(std::string const& cid, std::string const& url,
                                            std::string const& etag = std::string());

};