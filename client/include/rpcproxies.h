#ifndef RPCPROXIES_H
#define RPCPROXIES_H

#include <callback.h>

#ifdef ICE_CPP11_MAPPING

#include <condition_variable>
#include <mutex>
#include <thread>

class RpcProxies {
public:
    RpcProxies();

    void runHeartbeat();
    void startHeartbeat();

    int add(const std::shared_ptr<Uvss::CallbackSenderPrx>&, const std::string&);
    bool remove(int, std::string&, std::shared_ptr<Uvss::CallbackSenderPrx>&);
    bool has(const std::string&);
    int connectionId(const std::string&);

    void destroyHeartbeat();
    void joinHeartbeat();

private:
    std::map<std::shared_ptr<Uvss::CallbackSenderPrx>, std::string>
        proxyEndpointMap_;
    std::map<std::string, int> endpointConnectionIdMap_;
    int connectionId_;
    bool destroy_;

    std::mutex mutex_;
    std::condition_variable condition_;
    std::thread heartbeatThread_;
};

#else

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

class RpcProxies : public virtual IceUtil::Shared {
public:
    RpcProxies();

    void runHeartbeat();
    void startHeartbeat();

    int add(const Uvss::CallbackSenderPrx&, const std::string&);
    bool remove(int, std::string&, Uvss::CallbackSenderPrx&);
    bool has(const std::string&);
    int connectionId(const std::string&);

    void destroyHeartbeat();
    void joinHeartbeat();

    void sendCheckInfo(const std::vector<std::string>&,
                       const std::vector<std::string>&,
                       const std::vector<std::vector<unsigned char> >&);
    void sendCheckInfo(const std::string&,
                       const std::vector<std::string>&,
                       const std::vector<std::string>&,
                       const std::vector<std::vector<unsigned char> >&);

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

    std::map<Uvss::CallbackSenderPrx, std::string> proxyEndpointMap_;
    std::map<std::string, int> endpointConnectionIdMap_;
    int connectionId_;
    bool destroy_;

    boost::mutex mutex_;
    boost::condition_variable condition_;
    boost::thread heartbeatThread_;
};

#endif

#endif
