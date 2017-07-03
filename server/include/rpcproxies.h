#ifndef RPCPROXIES_H
#define RPCPROXIES_H

#include <map>

#ifdef ICE_CPP11_MAPPING
#include <mutex>
#include <condition_variable>
#include <thread>
#else
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread.hpp>
#endif

#include <callback.h>

#ifdef ICE_CPP11_MAPPING

class RpcProxies {
public:
    RpcProxies();

    void runHeartbeat();
    void startHeartbeat();

    void add(const std::shared_ptr<Uvss::CallbackReceiverPrx>&,
             const std::string&);
    void sendCheckInfo(const std::vector<std::string>&,
                       const std::vector<std::string>&,
                       const std::vector<std::vector<unsigned char>>&);

    void destroyHeartbeat();
    void joinHeartbeat();

private:
    std::map<std::shared_ptr<Uvss::CallbackReceiverPrx>, std::string>
        proxyEndpointMap_;
    bool destroy_;

    std::mutex mutex_;
    std::condition_variable condition_;
    std::thread heartbeatThread_;
};

#else

class RpcProxies : public virtual IceUtil::Shared {
public:
    RpcProxies();

    void runHeartbeat();
    void startHeartbeat();

    void add(const Uvss::CallbackReceiverPrx&,
             const std::string&);
    void sendCheckInfo(const std::vector<std::string>&,
                       const std::vector<std::string>&,
                       const std::vector<std::vector<unsigned char> >&);

    void destroyHeartbeat();
    void joinHeartbeat();

private:
    class Callback : public virtual IceUtil::Shared {
    public:
        void response()
        {
        }

        void exception(const Ice::Exception& ex)
        {
            std::cerr << "sendData AMI call failed:\n" << ex << std::endl;
        }
    };
    std::map<Uvss::CallbackReceiverPrx, std::string> proxyEndpointMap_;
    bool destroy_;

    boost::mutex mutex_;
    boost::condition_variable condition_;
    boost::thread heartbeatThread_;
};

#endif

#endif // RPCPROXIES_H
