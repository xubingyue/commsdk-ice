#ifndef SERVERI_H
#define SERVERI_H

#include <condition_variable>
#include <map>
#include <mutex>
#include <string>
#include <thread>

#include <Ice/Ice.h>

#include <clientserver.h>
#include <uvssserversdk.h>

class ServerI : public UVSS::Server {
public:
    ServerI();

    static void setConnectionInfoCallback(UVSSServerCallback);
    
    virtual void addClient(Ice::Identity, const Ice::Current&) override;
    virtual bool checkVersion(std::string, const Ice::Current&) override;

    void filePathToBinary(const std::string&, UVSS::ByteSeq&);
    const std::string createCurrentTime();
    void sendCheckInfo(const std::vector<std::string>&, const std::vector<std::string>&);
    
    void start();
    void destroy();

private:
    static UVSSServerCallback connectionInfoCallback;

    std::map<std::shared_ptr<UVSS::ClientPrx>, std::string> clientProxyToEndpoint;
    bool isDestroyed;
    
    std::mutex _mutex;
    std::condition_variable _cv;
    std::thread _senderThread;
};

#endif
