#pragma once

#include <msa/client/service_client.h>

class MsaDaemonManager {

public:
    static MsaDaemonManager instance;

    void set_msa_path(std::string const& path) {
        this->msa_path = path;
    }

    std::shared_ptr<msa::client::ServiceClient> connect_to_msa();

private:
    std::string msa_path;
    std::weak_ptr<msa::client::ServiceClient> msa_client;

};