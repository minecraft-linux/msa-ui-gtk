#include "MsaDaemonManager.h"

MsaDaemonManager MsaDaemonManager::instance;

std::shared_ptr<msa::client::ServiceClient> MsaDaemonManager::connect_to_msa() {
    auto client = msa_client.lock();
    if (client)
        return client;
    if (msa_path.empty())
        return nullptr;
    try {
        client = std::make_shared<msa::client::ServiceClient>(msa_path);
    } catch (std::exception& e) {
        return nullptr;
    }
    msa_client = client;
    return client;
}