#include <callbacksenderi.h>

#include <boost/lexical_cast.hpp>
#include <Ice/Ice.h>

#ifdef ICE_CPP11_MAPPING

CallbackSenderI::CallbackSenderI(const std::shared_ptr<RpcProxies>& proxies) :
    proxies_(proxies)
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

#else

CallbackSenderI::CallbackSenderI(const IceUtil::Handle<RpcProxies>& proxies) :
    proxies_(proxies)
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

#endif
