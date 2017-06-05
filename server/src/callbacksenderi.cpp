#include <callbacksenderi.h>

#include <fstream>
#include <memory>
#include <sstream>

#include <callback.h>
#include <version.h>

ServerI::ServerI(const std::shared_ptr<RpcExecutor>& workQueue) : _workQueue(workQueue)
{
}

void ServerI::addClient(Ice::Identity id, const Ice::Current& curr)
{
    _workQueue->add(id, curr);
}

bool ServerI::checkVersion(std::string ver, const Ice::Current&)
{
    return ver == UVSS_COMM_SDK_VER;
}
