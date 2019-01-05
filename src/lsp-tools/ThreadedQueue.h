#ifndef LANGUAGESERVER_TOOLS_MESSAGEQUEUE_H
#define LANGUAGESERVER_TOOLS_MESSAGEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include "library.h"

template <class T>
class ThreadedQueue {
private:
    mutable std::mutex mutex;
    std::deque<T> priority_queue;
    std::deque<T> queue;
    std::condition_variable c;
    size_t size { 0 };

public:
    bool empty () const { return size == 0; }

    void enqueue (T &&elem, bool priority = false) {
        std::cerr << "enqueuing message...\n";

        if (elem.IsNull()) {
            std::cerr << "message going in null :(...\n";
        }

        {
            std::lock_guard<std::mutex> lock(mutex);
            if (priority) {
                priority_queue.push_back(std::move(elem));
            } else {
                queue.push_back(std::move(elem));
            }
            size++;
        }
        c.notify_one();
    }

    T dequeue () {
        std::unique_lock<std::mutex> lock(mutex);
        c.wait(lock, [this]{ return size; });

        if (size <= 0) exit(5);

        T elem;

        if (priority_queue.empty()) {
            elem = std::move(queue.front());
            queue.pop_front();
        } else {
            elem = std::move(priority_queue.front());
            priority_queue.pop_front();
        }
        size--;

        std::cerr << "dequeuing message...\n";

//        sendMessage(elem);

        return std::move(elem);
    }
};


#endif //LANGUAGESERVER_TOOLS_MESSAGEQUEUE_H
