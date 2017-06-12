#include <peerproxies.h>

#include <boost/lexical_cast.hpp>
#include <Ice/Ice.h>

#include <global.h>

// UVSSServerCallback PeerProxies::connectionCallback_ = 0;

PeerProxies::PeerProxies() : destroy_(false)
{
}

void PeerProxies::run()
{
    while (true) {
        std::map<std::shared_ptr<Uvss::CallbackReceiverPrx>, std::string> clientEndpointMap;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait_for(lock, std::chrono::seconds(2));

            if (destroy_) {
                return;
            }
            else {
                clientEndpointMap = clientEndpointMap_;
            }
        }

        if (!clientEndpointMap.empty()) {
            for (auto p : clientEndpointMap) {
                try {
                    p.first->ice_ping();
                }
                catch (const Ice::Exception& ex) {
                    std::cerr << ex << std::endl;

//                     与C# GUI妥协的做法
//                     当destroy时，没有删除此刻失效的server代理、使用回调
//                     std::unique_lock<std::mutex> lock(mutex_);
//                     if (destroy_) {
//                         return;
//                     }
//                     else {
//                         auto client = p.first;
//                         std::string endpoint = p.second;
//                         std::string message("客户端 " + endpoint + ": 已断开");
//                         clientEndpointMap_.erase(client);
//                         connectionCallback_(-1, message.c_str());
//                     }

//                     正确做法
                    auto client = p.first;
                    std::string endpoint = p.second;
                    std::string message("客户端 " + endpoint + ": 已断开");

                    std::unique_lock<std::mutex> lock(mutex_); // 保证删除和回调通知一致
                    clientEndpointMap_.erase(client);
                    g_connectionCallback(-1, message.c_str());
                }
            }
        }
    }
}

void PeerProxies::start()
{
    std::thread t([this]()
    {
        run();
    });
    senderThread_ = std::move(t);
}

// void PeerProxies::add(const Ice::Identity& ident, const Ice::Current& current)
void PeerProxies::add(const std::shared_ptr<Uvss::CallbackReceiverPrx>& client, const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(mutex_);

    clientEndpointMap_[client] = endpoint;
    std::string message("客户端 " + endpoint + ": 已连接");
    g_connectionCallback(0, message.c_str());
}

void PeerProxies::sendCheckInfo(
    const std::vector<std::string>& strings,
    const std::vector<std::string>& fileNames,
    const std::vector<std::vector<unsigned char>>& files)
{
    std::unique_lock<std::mutex> lock(mutex_);

    for (auto p : clientEndpointMap_) {
        try {
            p.first->sendDataAsync(strings, fileNames, files,
                nullptr,
                [](std::exception_ptr e)
            {
                try {
                    std::rethrow_exception(e);
                }
                catch (const std::exception& ex) {
                    std::cerr << "sayHello AMI call failed:\n" << ex.what() << std::endl;
                }
            });
        }
        catch (const Ice::Exception& ex) {
            std::cerr << "sendCheckInfo:\n" << ex << std::endl;
//             不在此处删除失效的client代理
//             只让心跳线程检测对端连接和删除失效的client代理
//             若在此处删除失效的client代理：
//             1.若在这里回调通知，心跳线程也可能再回调通知一次（在此处删除失效代理后，若心跳线程还没有检测client副本里这个失效代理）
//             2.若不在这里回调通知，心跳线程可能会漏掉通知（在此处删除失效代理后，若心跳线程已经检测了client副本里这个代理，而检测时此代理是正常的）
        }
    }
}

void PeerProxies::destroy()
{
    std::unique_lock<std::mutex> lock(mutex_);
    destroy_ = true;
    cv_.notify_one();
}

void PeerProxies::join()
{
    if(senderThread_.joinable()) {
        senderThread_.join();
    }
}
