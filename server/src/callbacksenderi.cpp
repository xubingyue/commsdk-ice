#include <callbacksenderi.h>

#include <version.h>

CallbackSenderI::CallbackSenderI(const std::shared_ptr<PeerProxies>& peerProxies) :
    peerProxies_(peerProxies)
{
}

void CallbackSenderI::addClient(Ice::Identity ident, const Ice::Current& current)
{
    peerProxies_->add(ident, current);
}

bool CallbackSenderI::checkVersion(std::string version, const Ice::Current& /*current*/)
{
    return version == UVSS_COMM_SDK_VER;
}
