#include <rpcproxies.h>

#include <boost/lexical_cast.hpp>

#include <global.h>

#ifdef ICE_CPP11_MAPPING

RpcProxies::RpcProxies() : connectionId_(0), destroy_(false)
{
}

void RpcProxies::runHeartbeat()
{
    while (true) {
        std::map<std::shared_ptr<Uvss::CallbackSenderPrx>,
            std::string> proxyEndpointMap;
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
//                         std::string message("Server " + endpoint + ": " +
//                             "Disconnected | Connection Id: " +
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
        this->runHeartbeat();
    });
    heartbeatThread_ = std::move(t);
}

// connect后 add
int RpcProxies::add(const std::shared_ptr<Uvss::CallbackSenderPrx>& proxy,
                    const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(mutex_);

    proxyEndpointMap_[proxy] = endpoint;
    ++connectionId_;
    endpointConnectionIdMap_[endpoint] = connectionId_;
    return connectionId_;
}

// disconnect后 remove
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

void RpcProxies::sendCheckInfo(
    const std::vector<std::string>& strings,
    const std::vector<std::string>& fileNames,
    const std::vector<std::vector<unsigned char>>& files)
{
    std::unique_lock<std::mutex> lock(mutex_);

    for (auto p : proxyEndpointMap_) {
        try {
            p.first->sendDataAsync(strings, fileNames, files,
                nullptr,
                [](std::exception_ptr e)
            {
                try {
                    std::rethrow_exception(e);
                }
                catch (const std::exception& ex) {
                    std::cerr << "sendData AMI call failed:\n" <<
                        ex.what() << std::endl;
                }
            });
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

void RpcProxies::sendCheckInfo(
    const std::string& endpoint,
    const std::vector<std::string>& strings,
    const std::vector<std::string>& fileNames,
    const std::vector<std::vector<unsigned char>>& files)
{
    std::unique_lock<std::mutex> lock(mutex_);

    for (auto p : proxyEndpointMap_) {
        if (p.second == endpoint) {
            try {
                p.first->sendDataAsync(strings, fileNames, files,
                                       nullptr,
                                       [](std::exception_ptr e)
                {
                    try {
                        std::rethrow_exception(e);
                    }
                    catch (const std::exception& ex) {
                        std::cerr << "sendData AMI call failed:\n" <<
                                  ex.what() << std::endl;
                    }
                });
                break;
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

#else

RpcProxies::RpcProxies() : connectionId_(0), destroy_(false)
{
}

void RpcProxies::runHeartbeat()
{
    while (true) {
        std::map<Uvss::CallbackSenderPrx, std::string> proxyEndpointMap;
        {
            boost::unique_lock<boost::mutex> lock(mutex_);
            condition_.wait_for(lock, boost::chrono::seconds(2));

            if (destroy_) {
                return;
            }
            else {
//                 每次循环都应保持一致，无论proxyEndpointMap_是否为空！
                proxyEndpointMap = proxyEndpointMap_;
            }
        }

        if (!proxyEndpointMap.empty()) {
            for (std::map<Uvss::CallbackSenderPrx, std::string>::iterator
                p = proxyEndpointMap.begin(); p != proxyEndpointMap.end(); ++p) {
                try {
                    p->first->ice_ping();
                }
                catch (const Ice::Exception& ex) {
                    std::cerr << ex << std::endl;

//                     与C# GUI妥协的做法
//                     当destroy时，没有删除此刻失效的proxy、使用回调
//                     boost::unique_lock<boost::mutex> lock(mutex_);
//                     if (destroy_) {
//                         return;
//                     }
//                     else {
//                         Uvss::CallbackSenderPrx proxy = p->first;
//                         std::string endpoint = p->second;
//                         int connectionId = endpointConnectionIdMap_[endpoint];
//                         std::string message("Server " + endpoint + ": " +
//                             "Disconnected | Connection Id: " +
//                             boost::lexical_cast<std::string>(connectionId));
//                         proxyEndpointMap_.erase(proxy);
//                         endpointConnectionIdMap_.erase(endpoint);
//                         lock.unlock();
// 
//                         g_connectionCallback(connectionId, -3, message.c_str());
//                     }

//                     正确做法
                    Uvss::CallbackSenderPrx proxy = p->first;
                    std::string endpoint = p->second;
                    int connectionId = endpointConnectionIdMap_[endpoint];
                    std::string message("Server " + endpoint + ": " +
                        "Disconnected | Connection Id: " +
                        boost::lexical_cast<std::string>(connectionId));

                    boost::unique_lock<boost::mutex> lock(mutex_);
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
    boost::function0<void> f = boost::bind(&RpcProxies::runHeartbeat, this);
    boost::thread t(f);
    heartbeatThread_ = boost::move(t);
}

// connect后 add
int RpcProxies::add(const Uvss::CallbackSenderPrx& proxy,
                    const std::string& endpoint)
{
    boost::unique_lock<boost::mutex> lock(mutex_);

    proxyEndpointMap_[proxy] = endpoint;
    ++connectionId_;
    endpointConnectionIdMap_[endpoint] = connectionId_;
    return connectionId_;
}

// disconnect后 remove
bool RpcProxies::remove(int connectionId, std::string& endpoint,
                        Uvss::CallbackSenderPrx& proxy)
{
    boost::unique_lock<boost::mutex> lock(mutex_);

    for (std::map<std::string, int>::iterator
        x = endpointConnectionIdMap_.begin();
        x != endpointConnectionIdMap_.end(); ++x) {
        if (x->second == connectionId) {
            endpoint = x->first;
            endpointConnectionIdMap_.erase(endpoint);
            for (std::map<Uvss::CallbackSenderPrx, std::string>::iterator
                y = proxyEndpointMap_.begin();
                y != proxyEndpointMap_.end(); ++y) {
                if (y->second == endpoint) {
                    proxy = y->first;
                    proxyEndpointMap_.erase(y->first);

                    return true;
                }
            }
            std::cerr << "impossible?" << std::endl;
        }
    }

    return false;
}

bool RpcProxies::has(const std::string& endpoint)
{
    boost::unique_lock<boost::mutex> lock(mutex_);
    for (std::map<Uvss::CallbackSenderPrx, std::string>::iterator
        p = proxyEndpointMap_.begin(); p != proxyEndpointMap_.end(); ++p) {
        if (p->second == endpoint) {
            return true;
        }
    }

    return false;
}

int RpcProxies::connectionId(const std::string& endpoint)
{
    boost::unique_lock<boost::mutex> lock(mutex_);
    return endpointConnectionIdMap_[endpoint];
}

void RpcProxies::sendCheckInfo(
    const std::vector<std::string>& strings,
    const std::vector<std::string>& fileNames,
    const std::vector<std::vector<unsigned char> >& files)
{
    boost::unique_lock<boost::mutex> lock(mutex_);

    for (std::map<Uvss::CallbackSenderPrx, std::string>::iterator
        p = proxyEndpointMap_.begin(); p != proxyEndpointMap_.end(); ++p) {
        try {
            IceUtil::Handle<Callback> cb = new Callback;
            p->first->begin_sendData(strings, fileNames, files,
                Uvss::newCallback_CallbackSender_sendData(cb,
                &Callback::response, &Callback::exception));
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

void RpcProxies::sendCheckInfo(
    const std::string& endpoint,
    const std::vector<std::string>& strings,
    const std::vector<std::string>& fileNames,
    const std::vector<std::vector<unsigned char> >& files)
{
    boost::unique_lock<boost::mutex> lock(mutex_);

    for (std::map<Uvss::CallbackSenderPrx, std::string>::iterator
            p = proxyEndpointMap_.begin(); p != proxyEndpointMap_.end(); ++p) {
        if (p->second == endpoint) {
            try {
                IceUtil::Handle<Callback> cb = new Callback;
                p->first->begin_sendData(strings, fileNames, files,
                    Uvss::newCallback_CallbackSender_sendData(cb,
                    &Callback::response, &Callback::exception));
                break;
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
}

void RpcProxies::destroyHeartbeat()
{
    boost::unique_lock<boost::mutex> lock(mutex_);
    destroy_ = true;
    condition_.notify_one();
}

void RpcProxies::joinHeartbeat()
{
    if (heartbeatThread_.joinable()) {
        heartbeatThread_.join();
    }
}

#endif
