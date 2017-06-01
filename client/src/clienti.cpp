#include <clienti.h>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <clientserver.h>

#include <exception>

ClientI::ClientI(const std::shared_ptr<WorkQueue>& workQueue,
    const std::shared_ptr<RpcExecutor>& rpcExecutor)
    :  _workQueue(workQueue), _rpcExecutor(rpcExecutor)
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
