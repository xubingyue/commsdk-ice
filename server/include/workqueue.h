#ifndef WORKQUEUE_H
#define WORKQUEUE_H

#include <list>

#include <callback.h>

#ifdef ICE_CPP11_MAPPING

#include <condition_variable>
#include <mutex>
#include <thread>

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
        int,
        std::vector<std::string>&);
};

#else

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

class WorkQueue : public virtual IceUtil::Shared {
public:
    WorkQueue();

    void run();
    void start();

    void add(const Uvss::AMD_CallbackSender_sendDataPtr&,
             int,
             const std::vector<std::string>&,
             const std::vector<std::string>&,
             const std::vector<std::vector<unsigned char> >&);

    void destroy();
    void join();

private:
    struct CallbackEntry {
        Uvss::AMD_CallbackSender_sendDataPtr cb;
        int index;
        std::vector<std::string> strings;
        std::vector<std::string> fileNames;
        std::vector<std::vector<unsigned char> > files;
    };

    std::list<CallbackEntry> callbacks_;
    bool destroy_;

    boost::mutex mutex_;
    boost::condition_variable condition_;
    boost::thread workthread_;

    std::string fileDirectory(const std::string&);
    void fileNamesAndFilesTofilePaths(
        std::vector<std::string>&,
        const std::vector<std::vector<unsigned char> >&,
        const std::string&,
        int,
        std::vector<std::string>&);
};

#endif

#endif
