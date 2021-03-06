#include <callbacksenderi.h>

#include <boost/lexical_cast.hpp>
#include <Ice/Ice.h>

#ifdef ICE_CPP11_MAPPING

CallbackSenderI::CallbackSenderI(const std::shared_ptr<WorkQueue>& queue,
                                 const std::shared_ptr<RpcProxies>& proxies) :
    queue_(queue), proxies_(proxies)
{
}

void CallbackSenderI::addProxy(Ice::Identity ident, const Ice::Current& current)
{
    auto proxy = Ice::uncheckedCast<Uvss::CallbackReceiverPrx>(
        current.con->createProxy(ident));

//     std::cout << current.con->getEndpoint()->toString() << std::endl;
//     tcp -p 20145 -t 60000
//     std::cout << proxy->ice_getConnection()->getEndpoint()->toString() << std::endl;
//     tcp -p 20145 -t 60000
//     std::cout << proxy->ice_getEndpoints().size() << std::endl;
//     0

    Ice::ConnectionInfoPtr info = current.con->getInfo();
    Ice::TCPConnectionInfoPtr tcpInfo =
        std::dynamic_pointer_cast<Ice::TCPConnectionInfo>(info);
    std::string endpoint = tcpInfo->remoteAddress.replace(0, 7, "") + ":" +
        boost::lexical_cast<std::string>(tcpInfo->remotePort); // 去掉开头的::ffff:

    proxies_->add(proxy, endpoint);
}

bool CallbackSenderI::checkVersion(std::string version,
                                   const Ice::Current& /*current*/)
{
    return version == Uvss::version;
}

void CallbackSenderI::sendDataAsync(
    std::vector<std::string> strings,
    std::vector<std::string> fileNames,
    std::vector<std::vector<unsigned char>> files,
    std::function<void()> response,
    std::function<void(std::exception_ptr)> error,
    const Ice::Current& current)
{
//     简化endpoint形式？使用正则表达式？
//     std::cout << current.con->getEndpoint()->toString() << std::endl;
//     tcp -h 127.0.0.1 -p 20145 -t 60000
    Ice::ConnectionInfoPtr info = current.con->getInfo();
    Ice::TCPConnectionInfoPtr tcpInfo =
        std::dynamic_pointer_cast<Ice::TCPConnectionInfo>(info);
    std::string endpoint = tcpInfo->remoteAddress + ":" +
        boost::lexical_cast<std::string>(tcpInfo->remotePort);
//     std::cout << endpoint << std::endl;
    int connectionId = proxies_->connectionId(endpoint);

    queue_->add(connectionId, strings, fileNames, files,
                std::move(response), std::move(error));
}

#else

CallbackSenderI::CallbackSenderI(const IceUtil::Handle<WorkQueue>& queue,
                                 const IceUtil::Handle<RpcProxies>& proxies) :
    queue_(queue), proxies_(proxies)
{
}

void CallbackSenderI::addProxy(const Ice::Identity& ident,
                               const Ice::Current& current)
{
    Uvss::CallbackReceiverPrx proxy = Uvss::CallbackReceiverPrx::uncheckedCast(
        current.con->createProxy(ident));

//     std::cout << current.con->getEndpoint()->toString() << std::endl;
//     tcp -p 20145 -t 60000
//     std::cout << proxy->ice_getConnection()->getEndpoint()->toString() << std::endl;
//     tcp -p 20145 -t 60000
//     std::cout << proxy->ice_getEndpoints().size() << std::endl;
//     0

    Ice::ConnectionInfoPtr info = current.con->getInfo();
    Ice::TCPConnectionInfoPtr tcpInfo =
        Ice::TCPConnectionInfoPtr::dynamicCast(info);
    std::string endpoint = tcpInfo->remoteAddress.replace(0, 7, "") + ":" +
        boost::lexical_cast<std::string>(tcpInfo->remotePort); // 去掉开头的::ffff:

    proxies_->add(proxy, endpoint);
}

bool CallbackSenderI::checkVersion(const std::string& version,
                                   const Ice::Current& /*current*/)
{
    return version == Uvss::version;
}

void CallbackSenderI::sendData_async(
    const Uvss::AMD_CallbackSender_sendDataPtr& cb,
    const std::vector<std::string>& strings,
    const std::vector<std::string>& fileNames,
    const std::vector<std::vector<unsigned char> >& files,
    const Ice::Current& current)
{
//     简化endpoint形式？使用正则表达式？
//     std::cout << current.con->getEndpoint()->toString() << std::endl;
//     tcp -h 127.0.0.1 -p 20145 -t 60000
    Ice::ConnectionInfoPtr info = current.con->getInfo();
    Ice::TCPConnectionInfoPtr tcpInfo =
        Ice::TCPConnectionInfoPtr::dynamicCast(info);
    std::string endpoint = tcpInfo->remoteAddress + ":" +
        boost::lexical_cast<std::string>(tcpInfo->remotePort);
//     std::cout << endpoint << std::endl;
    int connectionId = proxies_->connectionId(endpoint);

    queue_->add(cb, connectionId, strings, fileNames, files);
}

#endif
