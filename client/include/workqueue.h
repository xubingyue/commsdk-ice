#ifndef WORKQUEUE_H
#define WORKQUEUE_H

#include <list>
#include <condition_variable>
#include <mutex>
#include <thread>

#include <callback.h>
#include <uvssclientwrapper.h>

class WorkQueue {
public:
    WorkQueue();

    void run();
    void start();

    void add(const std::vector<std::string>&,
             const std::vector<std::string>&,
             const std::vector<std::vector<unsigned char>>&,
             int,
             std::function<void ()>,
             std::function<void (std::exception_ptr)>);
    void createFileDirectory(const std::string&);

    void destroy();
    void join();

private:
    using CallbackEntry = std::tuple<
                          std::vector<std::string>,
                          std::vector<std::string>,
                          std::vector<std::vector<unsigned char>>,
                          int,
                          std::function<void ()>,
                          std::function<void (std::exception_ptr)>>;

    std::list<CallbackEntry> callbacks_;
    bool done_;

    std::mutex mutex_;
    std::condition_variable condition_;
    std::thread thread_;
};

#endif
