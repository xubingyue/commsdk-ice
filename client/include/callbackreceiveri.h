#ifndef CALLBACKRECEIVERI_H
#define CALLBACKRECEIVERI_H

#include <callback.h>
#include <rpcproxies.h>
#include <workqueue.h>

#ifdef ICE_CPP11_MAPPING

class CallbackReceiverI : public virtual Uvss::CallbackReceiver {
public:
    CallbackReceiverI(const std::shared_ptr<WorkQueue>&,
                      const std::shared_ptr<RpcProxies>&);

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

class CallbackReceiverI : public virtual Uvss::CallbackReceiver,
                          public virtual IceUtil::Shared {
public:
    CallbackReceiverI(const IceUtil::Handle<WorkQueue>&,
                      const IceUtil::Handle<RpcProxies>&);

    virtual void sendData_async(const Uvss::AMD_CallbackReceiver_sendDataPtr&,
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
