// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include <clientserver.h>

#include <list>
#include <condition_variable>
#include <mutex>
#include <thread>

#include <uvssclientsdk.h>

class WorkQueue
{
public:

    WorkQueue();

    void run();

    void add(std::string, UVSS::ByteSeq,
             std::string, UVSS::ByteSeq,
             std::string, std::string,
             std::string, std::string,
             std::string,
             std::function<void ()>, std::function<void (std::exception_ptr)>,
             int);
    void destroy();
    void start();
    void join();

    void createImageDirectory(const std::string&);

    static void setCheckInfoCallback(UVSSCheckInfoCallback);


private:

    using CallbackEntry = std::tuple<
                          std::string, UVSS::ByteSeq,
                          std::string, UVSS::ByteSeq,
                          std::string, std::string,
                          std::string, std::string,
                          std::string,
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
