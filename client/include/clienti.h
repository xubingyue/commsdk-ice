#ifndef CLIENTI_H
#define CLIENTI_H

#include <condition_variable>
#include <map>
#include <mutex>
#include <string>
#include <thread>

#include <Ice/Ice.h>

#include <clientserver.h>
#include <WorkQueue.h>

#include <memory>

typedef void (*ClientConnectionInfoCallback)(int, int, const char*);
typedef void (*ClientCheckInfoCallback)(int, const char*, const char*,
        const char*, const char*, const char*, const char*, const char*);

class ClientI : public UVSS::Client {
public:
    ClientI(const std::shared_ptr<WorkQueue>&);

    static void setConnectionInfoCallback(ClientConnectionInfoCallback);
    static void setCheckInfoCallback(ClientCheckInfoCallback);
    
    virtual void writeCheckInfoAsync(std::string, UVSS::ByteSeq,
                                std::string, UVSS::ByteSeq,
                                std::string,
                                std::string,
                                std::string,
                                std::string,
                                std::string,
                                std::function<void()>,
                                std::function<void(std::exception_ptr)>,
                                const Ice::Current& = Ice::noExplicitCurrent) override;

    void useConnectionInfoCallback(int, int, const std::string&);//考虑删除此函数
    void createImageDirectory(const std::string&);
    
    void start();
    void destroy();

    std::mutex _mutex;
    std::condition_variable _cv;
    int index;
    bool isDestroyed;
    std::map<std::shared_ptr<UVSS::ServerPrx>, std::string> serverProxyToEndpoint;
    std::map<std::string, int> endpointToIndex;
    
    std::thread _receiverThread;

    std::shared_ptr<WorkQueue> _workQueue;
    
private:
    static ClientConnectionInfoCallback connectionInfoCallback;
    static ClientCheckInfoCallback checkInfoCallback;
    
    
};

#endif
