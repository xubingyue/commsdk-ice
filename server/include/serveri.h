#ifndef SERVERI_H
#define SERVERI_H

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>

#include <Ice/Ice.h>

#include <clientserver.h>
#include <uvssserversdk.h>
#include <rpcexecutor.h>

class ServerI : public UVSS::Server {
public:
    ServerI(const std::shared_ptr<RpcExecutor>&);

    virtual void addClient(Ice::Identity, const Ice::Current&) override;
    virtual bool checkVersion(std::string, const Ice::Current&) override;

private:
    std::shared_ptr<RpcExecutor> _workQueue;
};

#endif
