#ifndef CALLBACKRECEIVERI_H
#define CALLBACKRECEIVERI_H

#include <callback.h>
#include <rpcproxies.h>
#include <workqueue.h>

#ifdef ICE_CPP11_MAPPING
class CallbackReceiverI : public virtual Uvss::CallbackReceiver {
public:
    CallbackReceiverI(const std::shared_ptr<RpcProxies>&,
                      const std::shared_ptr<WorkQueue>&);

    virtual void sendDataAsync(std::vector<std::string>,
                               std::vector<std::string>,
                               std::vector<std::vector<unsigned char>>,
                               std::function<void()>,
                               std::function<void(std::exception_ptr)>,
                               const Ice::Current&) override;

private:
    std::shared_ptr<RpcProxies> proxies_;
    std::shared_ptr<WorkQueue> workQueue_;
};
#else
class CallbackReceiverI : public virtual Uvss::CallbackReceiver, public virtual IceUtil::Shared {
public:
    CallbackReceiverI(const std::shared_ptr<RpcProxies>&,
                      const std::shared_ptr<WorkQueue>&);

    virtual void sendData_async(const Uvss::AMD_CallbackReceiver_sendDataPtr&,
        const std::vector<std::string>&,
                               const std::vector<std::string>&,
                               const std::vector<std::vector<unsigned char>>&,
                               const Ice::Current&);

private:
    std::shared_ptr<RpcProxies> proxies_;
    std::shared_ptr<WorkQueue> workQueue_;
};
#endif

#endif
