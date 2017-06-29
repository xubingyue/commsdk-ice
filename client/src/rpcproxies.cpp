#include <rpcproxies.h>

#include <boost/lexical_cast.hpp>

#include <global.h>

RpcProxies::RpcProxies() : connectionId_(0), destroy_(false)
{
}

void RpcProxies::runHeartbeat()
{
    while (true) {
#ifdef ICE_CPP11_MAPPING
        std::map<std::shared_ptr<Uvss::CallbackSenderPrx>,
            std::string> proxyEndpointMap;
#else
        std::map<Uvss::CallbackSenderPrx,
        std::string> proxyEndpointMap;
#endif
        {
            std::unique_lock<std::mutex> lock(mutex_);
            condition_.wait_for(lock, std::chrono::seconds(2));

            if (destroy_) {
                return;
            }
            else {
//                 每次循环都应保持一致，无论proxyEndpointMap_是否为空！
                proxyEndpointMap = proxyEndpointMap_;
            }
        }

        if (!proxyEndpointMap.empty()) {
            for (auto p : proxyEndpointMap) {
                try {
                    p.first->ice_ping();
                }
                catch (const Ice::Exception& ex) {
                    std::cerr << ex << std::endl;

//                     wrong! proxy已经失效
//                     std::cout << p.first->ice_getConnection()->getEndpoint()->toString() << std::endl;

//                     与C# GUI妥协的做法
//                     当destroy时，没有删除此刻失效的proxy、使用回调
//                     std::unique_lock<std::mutex> lock(mutex_);
//                     if (destroy_) {
//                         return;
//                     }
//                     else {
//                         auto proxy = p.first;
//                         std::string endpoint = p.second;
//                         int connectionId = endpointConnectionIdMap_[endpoint];
//                         std::string message("服务器端 " + endpoint + ": " +
//                             "已断开 | 连接标识: " +
//                             boost::lexical_cast<std::string>(connectionId));
//                         serverEndpointMap_.erase(proxy);
//                         endpointConnectionIdMap_.erase(endpoint);
//                         lock.unlock();
//
//                         g_connectionCallback(connectionId, -3, message.c_str());
//                     }

//                     正确做法
                    auto proxy = p.first;
                    std::string endpoint = p.second;
                    int connectionId = endpointConnectionIdMap_[endpoint];
                    std::string message("Server " + endpoint + ": " +
                        "Disconnected | Connection Id: " +
                        boost::lexical_cast<std::string>(connectionId));

                    std::unique_lock<std::mutex> lock(mutex_);
                    proxyEndpointMap_.erase(proxy);
                    endpointConnectionIdMap_.erase(endpoint);
                    lock.unlock();

                    g_connectionCallback(connectionId, -3, message.c_str());
                }
            }
        }
    }
}

void RpcProxies::startHeartbeat()
{
    std::thread t([this]() 
    {
        runHeartbeat();
    });
    heartbeatThread_ = std::move(t);
}

// connect后 add
#ifdef ICE_CPP11_MAPPING
int RpcProxies::add(const std::shared_ptr<Uvss::CallbackSenderPrx>& proxy,
                    const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(mutex_);

    proxyEndpointMap_[proxy] = endpoint;
    ++connectionId_;
    endpointConnectionIdMap_[endpoint] = connectionId_;
    return connectionId_;
}
#else
int RpcProxies::add(const Uvss::CallbackSenderPrx& proxy,
                    const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(mutex_);

    proxyEndpointMap_[proxy] = endpoint;
    ++connectionId_;
    endpointConnectionIdMap_[endpoint] = connectionId_;
    return connectionId_;
}
#endif

// disconnect后 remove
#ifdef ICE_CPP11_MAPPING
bool RpcProxies::remove(int connectionId, std::string& endpoint,
                               std::shared_ptr<Uvss::CallbackSenderPrx>& proxy)
{
    std::unique_lock<std::mutex> lock(mutex_);

    for (auto x : endpointConnectionIdMap_) {
        if (x.second == connectionId) {
            endpoint = x.first;
            endpointConnectionIdMap_.erase(endpoint);
            for (auto y : proxyEndpointMap_) {
                if (y.second == endpoint) {
                    proxy = y.first;
                    proxyEndpointMap_.erase(y.first);

                    return true;
                }
            }
            std::cerr << "impossible?" << std::endl;
        }
    }

    return false;
}
#else
bool RpcProxies::remove(int connectionId, std::string& endpoint,
                               Uvss::CallbackSenderPrx& proxy)
{
    std::unique_lock<std::mutex> lock(mutex_);

    for (auto x : endpointConnectionIdMap_) {
        if (x.second == connectionId) {
            endpoint = x.first;
            endpointConnectionIdMap_.erase(endpoint);
            for (auto y : proxyEndpointMap_) {
                if (y.second == endpoint) {
                    proxy = y.first;
                    proxyEndpointMap_.erase(y.first);

                    return true;
                }
            }
            std::cerr << "impossible?" << std::endl;
        }
    }

    return false;
}
#endif

bool RpcProxies::has(const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(mutex_);
    for (auto p : proxyEndpointMap_) {
        if (p.second == endpoint) {
            return true;
        }
    }

    return false;
}

int RpcProxies::connectionId(const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(mutex_);
    return endpointConnectionIdMap_[endpoint];
}

void RpcProxies::destroyHeartbeat()
{
    std::unique_lock<std::mutex> lock(mutex_);
    destroy_ = true;
    condition_.notify_one();
}

void RpcProxies::joinHeartbeat()
{
    if (heartbeatThread_.joinable()) {
        heartbeatThread_.join();
    }
}
