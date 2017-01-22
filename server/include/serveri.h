#ifndef SERVERI_H
#define SERVERI_H

#include <map>
#include <string>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <clientserver.h>

#include <thread>
#include <mutex>
#include <condition_variable>

// class ServerI;
// typedef IceUtil::Handle<ServerI> ServerIPtr;
typedef void (*ServerConnectionInfoCallback)(int, const char*);

class ServerI : /*virtual */public UVSS::Server/*, virtual public IceUtil::Thread,
        virtual public IceUtil::Monitor<IceUtil::Mutex>*/ {
public:
    ServerI();

    static void setConnectionInfoCallback(ServerConnectionInfoCallback);

//     virtual bool checkVersion(const std::string&,
//             const Ice::Current& = Ice::Current());
//     virtual void addClient(const Ice::Identity&,
//             const Ice::Current& = Ice::Current());
//    virtual void run();
    
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

//    std::map<UVSS::ClientPrx, std::string> clientProxyToEndpoint;
    std::map<std::shared_ptr<UVSS::ClientPrx>, std::string> clientProxyToEndpoint;
    bool isDestroyed;
    
    std::mutex _mutex;
    std::condition_variable _cv;
    std::thread _senderThread;
};

#endif
