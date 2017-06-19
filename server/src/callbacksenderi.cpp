#include <callbacksenderi.h>

#include <boost/lexical_cast.hpp>
#include <Ice/Ice.h>

CallbackSenderI::CallbackSenderI(const std::shared_ptr<RpcProxies>& proxies) :
    proxies_(proxies)
{
}

void CallbackSenderI::addProxy(Ice::Identity ident,
                                const Ice::Current& current)
{
    auto proxy = Ice::uncheckedCast<Uvss::CallbackReceiverPrx>(
        current.con->createProxy(ident));

//     std::cout << current.con->getEndpoint()->toString() << std::endl;
//     tcp -p 20145 -t 60000
//     auto v = proxy->ice_getEndpoints();
//     std::cout << v.size() << std::endl;
//     0
//     std::cout << proxy->ice_getConnection()->getEndpoint()->toString() << std::endl;
//     tcp -p 20145 -t 60000

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
