#ifndef CALLBACKSENDERI_H
#define CALLBACKSENDERI_H

#include <callback.h>
#include <rpcproxies.h>
#include <workqueue.h>

#ifdef ICE_CPP11_MAPPING

class CallbackSenderI : public virtual Uvss::CallbackSender {
public:
    CallbackSenderI(const std::shared_ptr<WorkQueue>&,
                    const std::shared_ptr<RpcProxies>&);

    virtual void addProxy(Ice::Identity, const Ice::Current&) override;
    virtual bool checkVersion(std::string, const Ice::Current&) override;
    virtual void sendDataAsync(std::vector<std::string>,
                               std::vector<std::string>,
                               std::vector<std::vector<unsigned char>>,
                               std::function<void()>,
                               std::function<void(std::exception_ptr)>,
                               const Ice::Current&) override;

private:
    std::shared_ptr<WorkQueue> queue_;
    std::shared_ptr<RpcProxies> proxies_;
};

#else

class CallbackSenderI : public virtual Uvss::CallbackSender,
                        public virtual IceUtil::Shared {
public:
    CallbackSenderI(const IceUtil::Handle<WorkQueue>&,
                    const IceUtil::Handle<RpcProxies>&);

    virtual void addProxy(const Ice::Identity&, const Ice::Current&);
    virtual bool checkVersion(const std::string&, const Ice::Current&);
    virtual void sendData_async(const Uvss::AMD_CallbackSender_sendDataPtr&,
                                const std::vector<std::string>&,
                                const std::vector<std::string>&,
                                const std::vector<std::vector<unsigned char> >&,
                                const Ice::Current&);

private:
    IceUtil::Handle<WorkQueue> queue_;
    IceUtil::Handle<RpcProxies> proxies_;
};

#endif

#endif
