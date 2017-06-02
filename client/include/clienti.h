#ifndef CLIENTI_H
#define CLIENTI_H

#include <Ice/Ice.h>
#include <clientserver.h>
#include <rpcexecutor.h>
#include <workqueue.h>

class ClientI : public UVSS::Client {
public:
    ClientI(const std::shared_ptr<RpcExecutor>&, const std::shared_ptr<WorkQueue>&);

    virtual void writeCheckInfoAsync(
                                UVSS::StringSeq,
                                UVSS::ByteSeqSeq,
                                UVSS::StringSeq,
                                std::function<void()>,
                                std::function<void(std::exception_ptr)>,
                                const Ice::Current&) override;

private:
    std::shared_ptr<RpcExecutor> _rpcExecutor;
    std::shared_ptr<WorkQueue> _workQueue;
};

#endif
