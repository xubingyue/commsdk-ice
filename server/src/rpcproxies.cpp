#include <rpcproxies.h>

#include <boost/lexical_cast.hpp>

#include <global.h>

RpcProxies::RpcProxies() : destroy_(false)
{
}

void RpcProxies::runHeartbeat()
{
    while (true) {
#ifdef ICE_CPP11_MAPPING
        std::map<std::shared_ptr<Uvss::CallbackReceiverPrx>,
            std::string> proxyEndpointMap;
#else
            std::map<Uvss::CallbackReceiverPrx,
            std::string> proxyEndpointMap;
#endif
        {
            std::unique_lock<std::mutex> lock(mutex_);
            condition_.wait_for(lock, std::chrono::seconds(2));

            if (destroy_) {
                return;
            }
            else {
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

//                     与C# GUI妥协的做法
//                     当destroy时，没有删除此刻失效的proxy、使用回调
//                     std::unique_lock<std::mutex> lock(mutex_);
//                     if (destroy_) {
//                         return;
//                     }
//                     else {
//                         auto proxy = p.first;
//                         std::string endpoint = p.second;
//                         std::string message("客户端 " + endpoint + ": 已断开");
//                         proxyEndpointMap_.erase(proxy);
//                         lock.unlock();
//
//                         g_connectionCallback(-1, message.c_str());
//                     }

//                     正确做法
                    auto proxy = p.first;
                    std::string endpoint = p.second;
                    std::string message("Client " + endpoint +
                        ": Disconnected");

                    std::unique_lock<std::mutex> lock(mutex_); // 保证删除和回调通知一致
                    proxyEndpointMap_.erase(proxy);
                    lock.unlock();

                    g_connectionCallback(-1, message.c_str());
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

#ifdef ICE_CPP11_MAPPING
void RpcProxies::add(const std::shared_ptr<Uvss::CallbackReceiverPrx>& proxy,
                     const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(mutex_);
    proxyEndpointMap_[proxy] = endpoint;
    std::string message("Client " + endpoint + ": Connected");
    lock.unlock();

    g_connectionCallback(0, message.c_str());
}
#else
void RpcProxies::add(const Uvss::CallbackReceiverPrx& proxy,
                     const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(mutex_);
    proxyEndpointMap_[proxy] = endpoint;
    std::string message("Client " + endpoint + ": Connected");
    lock.unlock();

    g_connectionCallback(0, message.c_str());
}
#endif

void RpcProxies::sendCheckInfo(
    const std::vector<std::string>& strings,
    const std::vector<std::string>& fileNames,
    const std::vector<std::vector<unsigned char>>& files)
{
    std::unique_lock<std::mutex> lock(mutex_);

    for (auto p : proxyEndpointMap_) {
        try {
#ifdef ICE_CPP11_MAPPING
            p.first->sendDataAsync(strings, fileNames, files,
                nullptr,
                [](std::exception_ptr e)
            {
                try {
                    std::rethrow_exception(e);
                }
                catch (const std::exception& ex) {
                    std::cerr << "sayHello AMI call failed:\n" <<
                        ex.what() << std::endl;
                }
            });
#else
            IceUtil::Handle<Callback> cb = new Callback();
            p.first->begin_sendData(strings, fileNames, files, Uvss::newCallback_CallbackReceiver_sendData(cb, &Callback::response, &Callback::exception));
#endif
        }
        catch (const Ice::Exception& ex) {
            std::cerr << "sendCheckInfo:\n" << ex << std::endl;
//             不在此处删除失效proxy
//             只让心跳线程检测对端连接和删除失效proxy
//             若在此处删除失效proxy
//             1.若在此处回调通知，心跳线程也可能再回调通知一次（在此处删除失效proxy后，若心跳线程还没有检测proxy副本）
//             2.若不在此处回调通知，心跳线程可能会漏掉通知（在此处删除失效proxy后，若心跳线程已经检测了proxy副本，而检测时此proxy是正常的）
        }
    }
}

void RpcProxies::destroyHeartbeat()
{
    std::unique_lock<std::mutex> lock(mutex_);
    destroy_ = true;
    condition_.notify_one();
}

void RpcProxies::joinHeartbeat()
{
    if(heartbeatThread_.joinable()) {
        heartbeatThread_.join();
    }
}
