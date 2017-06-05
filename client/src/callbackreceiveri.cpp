#include <callbackreceiveri.h>

#include <exception>

#include <callback.h>

CallbackReceiverI::CallbackReceiverI(const std::shared_ptr<PeerProxies>& rpcExecutor,
                 const std::shared_ptr<WorkQueue>& workQueue)
    : _rpcExecutor(rpcExecutor), _workQueue(workQueue)
{
}

void CallbackReceiverI::sendCheckInfoAsync(
    UVSS::StringSeq filenames,
    UVSS::ByteSeqSeq files,
    UVSS::StringSeq strings,
    std::function<void()> response,
    std::function<void(std::exception_ptr)> error,
    const Ice::Current& current)
{
    int index = _rpcExecutor->serverIndex(current);

    _workQueue->add(filenames,
                    files,
                    strings,
                    move(response),
                    move(error),
                    index);
}
