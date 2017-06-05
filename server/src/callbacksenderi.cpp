#include <callbacksenderi.h>

#include <fstream>
#include <memory>
#include <sstream>

#include <callback.h>
#include <version.h>

CallbackSenderI::CallbackSenderI(const std::shared_ptr<PeerProxies>& workQueue) : _workQueue(workQueue)
{
}

void CallbackSenderI::addClient(Ice::Identity ident, const Ice::Current& current)
{
    _workQueue->add(ident, current);
}

bool CallbackSenderI::checkVersion(std::string version, const Ice::Current& /*current*/)
{
    return version == UVSS_COMM_SDK_VER;
}
