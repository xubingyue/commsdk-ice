#include <callbackreceiveri.h>

CallbackReceiverI::CallbackReceiverI(const std::shared_ptr<PeerProxies>& rpcExecutor,
                 const std::shared_ptr<WorkQueue>& workQueue)
    : peerProxies_(rpcExecutor), workQueue_(workQueue)
{
}

void CallbackReceiverI::sendCheckInfoAsync(
    Uvss::StringSeq filenames,
    Uvss::ByteSeqSeq files,
    Uvss::StringSeq strings,
    std::function<void()> response,
    std::function<void(std::exception_ptr)> error,
    const Ice::Current& current)
{
    int index = peerProxies_->serverIndex(current);

    workQueue_->add(filenames,
                    files,
                    strings,
                    move(response),
                    move(error),
                    index);
}
