#ifndef CALLBACKRECEIVERI_H
#define CALLBACKRECEIVERI_H

#include <callback.h>
#include <peerproxies.h>
#include <workqueue.h>

class CallbackReceiverI : public virtual Uvss::CallbackReceiver {
public:
    CallbackReceiverI(const std::shared_ptr<PeerProxies>&, const std::shared_ptr<WorkQueue>&);

    virtual void sendCheckInfoAsync(
        std::vector<std::string>,
        std::vector<std::vector<unsigned char>>,
        std::vector<std::string>,
        std::function<void()>,
        std::function<void(std::exception_ptr)>,
        const Ice::Current&) override;

private:
    std::shared_ptr<PeerProxies> peerProxies_;
    std::shared_ptr<WorkQueue> workQueue_;
};

#endif
