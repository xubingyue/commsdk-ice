#ifndef CLIENTI_H
#define CLIENTI_H

#include <condition_variable>
#include <map>
#include <mutex>
#include <string>
#include <thread>

#include <Ice/Ice.h>

#include <clientserver.h>

typedef void (*ClientConnectionInfoCallback)(int, int, const char*);
typedef void (*ClientCheckInfoCallback)(int, const char*, const char*,
        const char*, const char*, const char*, const char*, const char*);

class ClientI : public UVSS::Client {
public:
    ClientI();

    static void setConnectionInfoCallback(ClientConnectionInfoCallback);
    static void setCheckInfoCallback(ClientCheckInfoCallback);
    
    virtual void writeCheckInfo(std::string, UVSS::ByteSeq,
                                std::string, UVSS::ByteSeq,
                                std::string,
                                std::string,
                                std::string,
                                std::string,
                                std::string,
                                const Ice::Current& = Ice::noExplicitCurrent) override;

    void useConnectionInfoCallback(int, int, const std::string&);//考虑删除此函数
    void createImageDirectory(const std::string&);
    
    void start();
    void destroy();

    int index;
    bool isDestroyed;
    std::map<std::shared_ptr<UVSS::ServerPrx>, std::string> serverProxyToEndpoint;
    std::map<std::string, int> endpointToIndex;
    
    std::mutex _mutex;
    std::condition_variable _cv;
    std::thread _receiverThread;

private:
    static ClientConnectionInfoCallback connectionInfoCallback;
    static ClientCheckInfoCallback checkInfoCallback;
};

#endif
