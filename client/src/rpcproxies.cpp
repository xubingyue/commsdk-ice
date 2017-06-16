#include <rpcproxies.h>

#include <boost/lexical_cast.hpp>
#include <Ice/Ice.h>

#include <global.h>

RpcProxies::RpcProxies() : connectionId_(0), destroy_(false)
{
}

void RpcProxies::run()
{
    while (true) {
        std::map<std::shared_ptr<Uvss::CallbackSenderPrx>, std::string> serverEndpointMap;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait_for(lock, std::chrono::seconds(2));

            if (destroy_) {
                return;
            }
            else {
//                 每次循环都应保持一致，无论serverProxyToEndpoint是否为空！
                serverEndpointMap = serverEndpointMap_;
            }
        }

        if (!serverEndpointMap.empty()) {
            for (auto p : serverEndpointMap) {
                try {
                    p.first->ice_ping();
                }
                catch (const Ice::Exception& ex) {
                    std::cerr << ex << std::endl;

//                     wrong!server代理已经失效
//                     std::cout << p.first->ice_getConnection()->getEndpoint()->toString() << std::endl;

//                     与C# GUI妥协的做法
//                     当destroy时，没有删除此刻失效的server代理、使用回调
//                     std::unique_lock<std::mutex> lock(mutex_);
//                     if (destroy_) {
//                         return;
//                     }
//                     else {
//                         auto server = p.first;
//                         std::string endpoint = p.second;
//                         int connectionId = endpointConnectionIdMap_[endpoint];
//                         std::string message("服务器端 " + endpoint + ": " +
//                                             "已断开 | 连接标识: " + boost::lexical_cast<std::string>(connectionId));
//                         serverEndpointMap_.erase(server);
//                         endpointConnectionIdMap_.erase(endpoint);
//                         connectionCallback_(connectionId, -3, message.c_str());
//                     }


//                     正确做法
                    auto server = p.first;
                    std::string endpoint = p.second;
                    int connectionId = endpointConnectionIdMap_[endpoint];
                    std::string message("Server " + endpoint + ": " +
                                        "Disconnected | Connection Id: " + boost::lexical_cast<std::string>(connectionId));

                    std::unique_lock<std::mutex> lock(mutex_);
                    serverEndpointMap_.erase(server);
                    endpointConnectionIdMap_.erase(endpoint);
                    g_connectionCallback(connectionId, -3, message.c_str());
                }
            }
        }
    }
}

void RpcProxies::start()
{
    std::thread t([this]() 
    {
        run();
    });
    senderThread_ = std::move(t);
}

// 连接成功后 add!
int RpcProxies::add(const std::shared_ptr<Uvss::CallbackSenderPrx>& server, const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(mutex_);

    serverEndpointMap_[server] = endpoint;
    ++connectionId_;
    endpointConnectionIdMap_[endpoint] = connectionId_;
    return connectionId_;
}

// 断开连接后 findAndRemove!
bool RpcProxies::findAndRemove(int connectionId, std::string& endpoint, std::shared_ptr<Uvss::CallbackSenderPrx>& server)
{
    std::unique_lock<std::mutex> lock(mutex_);

    for (auto x : endpointConnectionIdMap_) {
        if (x.second == connectionId) {
            endpoint = x.first;
            endpointConnectionIdMap_.erase(endpoint);
            for (auto y : serverEndpointMap_) {
                if (y.second == endpoint) {
                    server = y.first;
                    serverEndpointMap_.erase(y.first); // client不再连接此server
                    return true;
                }
            }
            std::cerr << "impossible?" << std::endl;
        }
    }

    return false; // 没有此连接
}

bool RpcProxies::isRepeated(const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(mutex_);
    for (auto p : serverEndpointMap_) {
        if (p.second == endpoint) {
            return true;
        }
    }
    return false;
}

int RpcProxies::serverConnectionId(const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(mutex_);
    return endpointConnectionIdMap_[endpoint];
}

void RpcProxies::destroy()
{
    std::unique_lock<std::mutex> lock(mutex_);
    destroy_ = true;
    cv_.notify_one();
}

void RpcProxies::join()
{
    if (senderThread_.joinable()) {
        senderThread_.join();
    }
}
