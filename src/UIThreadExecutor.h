#pragma once

#include <functional>
#include <queue>
#include <mutex>
#include <giomm.h>

class UIThreadExecutor {

private:
    Glib::Dispatcher dispatcher;
    std::queue<std::function<void ()>> queue;
    std::mutex mutex;

    void on_signaled();

public:
    UIThreadExecutor();

    void run(std::function<void ()> func) {
        std::unique_lock<std::mutex> lock (mutex);
        queue.push(std::move(func));
        dispatcher.emit();
    }

};