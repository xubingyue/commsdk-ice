#ifndef WORKQUEUE_H
#define WORKQUEUE_H

#include <list>
#include <condition_variable>
#include <mutex>
#include <thread>

#include <callback.h>
#include <uvssclientwrapper.h>

class WorkQueue
{
public:

    WorkQueue();

    void run();

    void add(
        Uvss::StringSeq,
        Uvss::ByteSeqSeq,
        Uvss::StringSeq,
        std::function<void ()>, std::function<void (std::exception_ptr)>,
        int);
    void destroy();
    void start();
    void join();

    void createImageDirectory(const std::string&);

    static void setCheckInfoCallback(UVSSCheckInfoCallback);//-

private:

    using CallbackEntry = std::tuple<
                          Uvss::StringSeq,
                          Uvss::ByteSeqSeq,
                          Uvss::StringSeq,
                          std::function<void ()>,
                          std::function<void (std::exception_ptr)>,
                          int>;

    std::mutex _mutex;
    std::condition_variable _condition;

    std::list<CallbackEntry> _callbacks;
    bool _done;
    std::thread _thread;

    static UVSSCheckInfoCallback checkInfoCallback;
};

#endif
