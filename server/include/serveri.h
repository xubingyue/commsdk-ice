#ifndef SERVERI_H
#define SERVERI_H

#include <condition_variable>
#include <map>
#include <mutex>
#include <string>
#include <thread>

#include <Ice/Ice.h>

#include <clientserver.h>

typedef void (*ServerConnectionInfoCallback)(int, const char*);

class ServerI : public UVSS::Server {
public:
    ServerI();

    static void setConnectionInfoCallback(ServerConnectionInfoCallback);
    
    virtual void addClient(Ice::Identity, const Ice::Current& = Ice::noExplicitCurrent) override;
    virtual bool checkVersion(std::string, const Ice::Current& = Ice::noExplicitCurrent) override;

    void filePathToBinary(const std::string&, UVSS::ByteSeq&);
    const std::string createCurrentTime();
    void sendCheckInfo(const std::string&, const std::string&,
            const std::string&, const std::string&, const std::string&,
            const std::string&, const std::string&);
    
    void start();
    void destroy();

private:
    static ServerConnectionInfoCallback connectionInfoCallback;

    std::map<std::shared_ptr<UVSS::ClientPrx>, std::string> clientProxyToEndpoint;
    bool isDestroyed;
    
    std::mutex _mutex;
    std::condition_variable _cv;
    std::thread _senderThread;
};

#endif
