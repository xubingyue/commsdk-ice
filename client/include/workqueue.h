#ifndef WORKQUEUE_H
#define WORKQUEUE_H

#include <condition_variable>
#include <list>
#include <mutex>
#include <thread>

#include <callback.h>

#ifdef ICE_CPP11_MAPPING
class WorkQueue {
public:
    WorkQueue();

    void run();
    void start();

    void add(int,
             const std::vector<std::string>&,
             const std::vector<std::string>&,
             const std::vector<std::vector<unsigned char>>&,
             std::function<void ()>,
             std::function<void (std::exception_ptr)>);

    void destroy();
    void join();

private:
    using CallbackEntry = std::tuple<int,
                          std::vector<std::string>,
                          std::vector<std::string>,
                          std::vector<std::vector<unsigned char>>,
                          std::function<void ()>,
                          std::function<void (std::exception_ptr)>>;

    std::list<CallbackEntry> callbacks_;
    bool destroy_;

    std::mutex mutex_;
    std::condition_variable condition_;
    std::thread workthread_;

    std::string fileDirectory(const std::string&);
    void fileNamesAndFilesTofilePaths(
        std::vector<std::string>&,
        const std::vector<std::vector<unsigned char>>&,
        const std::string&,
        std::vector<std::string>&);
};
#else
class WorkQueue {
public:
    WorkQueue();

    void run();
    void start();

    void add(const Uvss::AMD_CallbackReceiver_sendDataPtr&,
             int,
             const std::vector<std::string>&,
             const std::vector<std::string>&,
             const std::vector<std::vector<unsigned char>>&);

    void destroy();
    void join();

private:
    struct CallbackEntry {
        Uvss::AMD_CallbackReceiver_sendDataPtr cb;
        int index;
        std::vector<std::string> strings;
        std::vector<std::string> fileNames;
        std::vector<std::vector<unsigned char>> files;
    };
    std::list<CallbackEntry> callbacks_;
    bool destroy_;

    std::mutex mutex_;
    std::condition_variable condition_;
    std::thread workthread_;

    std::string fileDirectory(const std::string&);
    void fileNamesAndFilesTofilePaths(
        std::vector<std::string>&,
        const std::vector<std::vector<unsigned char>>&,
        const std::string&,
        std::vector<std::string>&);
};
#endif

#endif
