#include <callbackreceiveri.h>

CallbackReceiverI::CallbackReceiverI(const std::shared_ptr<PeerProxies>& rpcExecutor,
                 const std::shared_ptr<WorkQueue>& workQueue)
    : peerProxies_(rpcExecutor), _workQueue(workQueue)
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
    int index = peerProxies_->serverIndex(current);

    _workQueue->add(filenames,
                    files,
                    strings,
                    move(response),
                    move(error),
                    index);
}
