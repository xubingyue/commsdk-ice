#include <clienti.h>

#include <exception>

#include <clientserver.h>

ClientI::ClientI(const std::shared_ptr<RpcExecutor>& rpcExecutor,
                 const std::shared_ptr<WorkQueue>& workQueue)
    : _rpcExecutor(rpcExecutor), _workQueue(workQueue)
{
}

void ClientI::writeCheckInfoAsync(
    UVSS::StringSeq ns,
    UVSS::ByteSeqSeq bss,
    UVSS::StringSeq ss,
    std::function<void()> response,
    std::function<void(std::exception_ptr)> error,
    const Ice::Current& curr)
{
    int index = _rpcExecutor->serverIndex(curr);

    _workQueue->add(ns,
                    bss,
                    ss,
                    move(response),
                    move(error),
                    index);
}
