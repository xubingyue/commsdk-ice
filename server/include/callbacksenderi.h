#ifndef CALLBACKSENDERI_H
#define CALLBACKSENDERI_H

#include <callback.h>
#include <rpcproxies.h>

#ifdef ICE_CPP11_MAPPING

class CallbackSenderI : public virtual Uvss::CallbackSender {
public:
    CallbackSenderI(const std::shared_ptr<RpcProxies>&);

    virtual void addProxy(Ice::Identity, const Ice::Current&) override;
    virtual bool checkVersion(std::string, const Ice::Current&) override;

private:
    std::shared_ptr<RpcProxies> proxies_;
};

#else

class CallbackSenderI : public virtual Uvss::CallbackSender, public virtual IceUtil::Shared {
public:
    CallbackSenderI(const IceUtil::Handle<RpcProxies>&);

    virtual void addProxy(const Ice::Identity&, const Ice::Current&);
    virtual bool checkVersion(const std::string&, const Ice::Current&);

private:
    IceUtil::Handle<RpcProxies> proxies_;
};

#endif

#endif
