#include <callbackreceiveri.h>

#include <boost/lexical_cast.hpp>
#include <Ice/Ice.h>

CallbackReceiverI::CallbackReceiverI(const std::shared_ptr<PeerProxies>& peerProxies,
                                     const std::shared_ptr<WorkQueue>& workQueue) :
    peerProxies_(peerProxies), workQueue_(workQueue)
{
}

void CallbackReceiverI::sendDataAsync(
    std::vector<std::string> strings,
    std::vector<std::string> fileNames,
    std::vector<std::vector<unsigned char>> files,
    std::function<void()> response,
    std::function<void(std::exception_ptr)> error,
    const Ice::Current& current)
{
//             简化endpoint形式？使用正则表达式？
//             std::cout << current.con->getEndpoint()->toString() << std::endl;
//             tcp -h 127.0.0.1 -p 20145 -t 60000
    Ice::ConnectionInfoPtr info = current.con->getInfo();
    Ice::TCPConnectionInfoPtr tcpInfo = std::dynamic_pointer_cast<Ice::TCPConnectionInfo>(info);
    std::string endpoint = tcpInfo->remoteAddress + ":" + boost::lexical_cast<std::string>(tcpInfo->remotePort);
//             std::cout << endpoint << std::endl;
    int connectionId = peerProxies_->serverConnectionId(endpoint);

    workQueue_->add(strings, fileNames, files, connectionId,
                    std::move(response), std::move(error));
}
