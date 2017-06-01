#ifndef CLIENTI_H
#define CLIENTI_H

#include <Ice/Ice.h>

#include <clientserver.h>
#include <workqueue.h>

#include <rpcexecutor.h>

class ClientI : public UVSS::Client {
public:
    ClientI(const std::shared_ptr<WorkQueue>&, const std::shared_ptr<RpcExecutor>&);

    virtual void writeCheckInfoAsync(
                                UVSS::StringSeq,
                                UVSS::ByteSeqSeq,
                                UVSS::StringSeq,
                                std::function<void()>,
                                std::function<void(std::exception_ptr)>,
                                const Ice::Current&) override;

    std::shared_ptr<WorkQueue> _workQueue;
    std::shared_ptr<RpcExecutor> _rpcExecutor;

private:
    
};

#endif
