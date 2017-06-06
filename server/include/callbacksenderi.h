#ifndef CALLBACKSENDERI_H
#define CALLBACKSENDERI_H

#include <callback.h>
#include <peerproxies.h>

class CallbackSenderI : public virtual Uvss::CallbackSender {
public:
    CallbackSenderI(const std::shared_ptr<PeerProxies>&);

    virtual void addClient(Ice::Identity, const Ice::Current&) override;
    virtual bool checkVersion(std::string, const Ice::Current&) override;

private:
    std::shared_ptr<PeerProxies> peerProxies_;
};

#endif
