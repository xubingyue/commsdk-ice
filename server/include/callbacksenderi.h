#ifndef CALLBACKSENDERI_H
#define CALLBACKSENDERI_H

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>

#include <Ice/Ice.h>

#include <callback.h>
#include <uvssserverwrapper.h>
#include <peerproxies.h>

class CallbackSenderI : public virtual UVSS::CallbackSender {
public:
    CallbackSenderI(const std::shared_ptr<PeerProxies>&);

    virtual void addClient(Ice::Identity, const Ice::Current&) override;
    virtual bool checkVersion(std::string, const Ice::Current&) override;

private:
    std::shared_ptr<PeerProxies> _workQueue;
};

#endif
