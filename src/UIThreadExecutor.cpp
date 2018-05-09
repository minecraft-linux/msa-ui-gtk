#include "UIThreadExecutor.h"

UIThreadExecutor::UIThreadExecutor() {
    dispatcher.connect(sigc::mem_fun(*this, &UIThreadExecutor::on_signaled));
}

void UIThreadExecutor::on_signaled() {
    std::queue<std::function<void ()>> queue;
    {
        std::unique_lock<std::mutex> lock(mutex);
        queue = std::move(this->queue);
    }
    while (!queue.empty()) {
        queue.front()();
        queue.pop();
    }
}