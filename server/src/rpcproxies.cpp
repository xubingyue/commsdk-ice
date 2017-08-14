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
        std::map<std::shared_ptr<Uvss::CallbackReceiverPrx>,
            std::string> proxyEndpointMap;
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

//                     wrong! proxy�Ѿ�ʧЧ
//                     std::cout << p.first->ice_getConnection()->getEndpoint()->toString() << std::endl;

                    auto proxy = p.first;
                    std::string endpoint = p.second;
                    std::string message("Client " + endpoint +
                        ": Disconnected");

                    std::unique_lock<std::mutex> lock(mutex_); // ��֤ɾ���ͻص�֪ͨһ��
                    proxyEndpointMap_.erase(proxy);
                    endpointConnectionIdMap_.erase(endpoint);
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
        this->runHeartbeat();
    });
    heartbeatThread_ = std::move(t);
}

void RpcProxies::add(const std::shared_ptr<Uvss::CallbackReceiverPrx>& proxy,
                     const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(mutex_);

    proxyEndpointMap_[proxy] = endpoint;
    ++connectionId_;
    endpointConnectionIdMap_[endpoint] = connectionId_;
    std::string message("Client " + endpoint + ": Connected");
    lock.unlock();

    g_connectionCallback(0, message.c_str());
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
//             ���ڴ˴�ɾ��ʧЧproxy
//             ֻ�������̼߳��Զ����Ӻ�ɾ��ʧЧproxy
//             ���ڴ˴�ɾ��ʧЧproxy
//             1.���ڴ˴��ص�֪ͨ�������߳�Ҳ�����ٻص�֪ͨһ�Σ��ڴ˴�ɾ��ʧЧproxy���������̻߳�û�м��proxy������
//             2.�����ڴ˴��ص�֪ͨ�������߳̿��ܻ�©��֪ͨ���ڴ˴�ɾ��ʧЧproxy���������߳��Ѿ������proxy�����������ʱ��proxy�������ģ�
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
//             ���ڴ˴�ɾ��ʧЧproxy
//             ֻ�������̼߳��Զ����Ӻ�ɾ��ʧЧproxy
//             ���ڴ˴�ɾ��ʧЧproxy
//             1.���ڴ˴��ص�֪ͨ�������߳�Ҳ�����ٻص�֪ͨһ�Σ��ڴ˴�ɾ��ʧЧproxy���������̻߳�û�м��proxy������
//             2.�����ڴ˴��ص�֪ͨ�������߳̿��ܻ�©��֪ͨ���ڴ˴�ɾ��ʧЧproxy���������߳��Ѿ������proxy�����������ʱ��proxy�������ģ�
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
    if(heartbeatThread_.joinable()) {
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
        std::map<Uvss::CallbackReceiverPrx, std::string> proxyEndpointMap;
        {
            boost::unique_lock<boost::mutex> lock(mutex_);
            condition_.wait_for(lock, boost::chrono::seconds(2));

            if (destroy_) {
                return;
            }
            else {
                proxyEndpointMap = proxyEndpointMap_;
            }
        }

        if (!proxyEndpointMap.empty()) {
            for (std::map<Uvss::CallbackReceiverPrx, std::string>::iterator
                p = proxyEndpointMap.begin(); p != proxyEndpointMap.end(); ++p) {
                try {
                    p->first->ice_ping();
                }
                catch (const Ice::Exception& ex) {
                    std::cerr << ex << std::endl;

//                     wrong! proxy�Ѿ�ʧЧ
//                     std::cout << p->first->ice_getConnection()->getEndpoint()->toString() << std::endl;

                    Uvss::CallbackReceiverPrx proxy = p->first;
                    std::string endpoint = p->second;
                    std::string message("Client " + endpoint +
                        ": Disconnected");

                    boost::unique_lock<boost::mutex> lock(mutex_); // ��֤ɾ���ͻص�֪ͨһ��
                    proxyEndpointMap_.erase(proxy);
                    endpointConnectionIdMap_.erase(endpoint);
                    lock.unlock();

                    g_connectionCallback(-1, message.c_str());
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

void RpcProxies::add(const Uvss::CallbackReceiverPrx& proxy,
                     const std::string& endpoint)
{
    boost::unique_lock<boost::mutex> lock(mutex_);

    proxyEndpointMap_[proxy] = endpoint;
    ++connectionId_;
    endpointConnectionIdMap_[endpoint] = connectionId_;
    std::string message("Client " + endpoint + ": Connected");
    lock.unlock();

    g_connectionCallback(0, message.c_str());
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

    for (std::map<Uvss::CallbackReceiverPrx, std::string>::iterator
        p = proxyEndpointMap_.begin(); p != proxyEndpointMap_.end(); ++p) {
        try {
            IceUtil::Handle<Callback> cb = new Callback;
            p->first->begin_sendData(strings, fileNames, files,
                Uvss::newCallback_CallbackReceiver_sendData(cb,
                &Callback::response, &Callback::exception));
        }
        catch (const Ice::Exception& ex) {
            std::cerr << "sendCheckInfo:\n" << ex << std::endl;
//             ���ڴ˴�ɾ��ʧЧproxy
//             ֻ�������̼߳��Զ����Ӻ�ɾ��ʧЧproxy
//             ���ڴ˴�ɾ��ʧЧproxy
//             1.���ڴ˴��ص�֪ͨ�������߳�Ҳ�����ٻص�֪ͨһ�Σ��ڴ˴�ɾ��ʧЧproxy���������̻߳�û�м��proxy������
//             2.�����ڴ˴��ص�֪ͨ�������߳̿��ܻ�©��֪ͨ���ڴ˴�ɾ��ʧЧproxy���������߳��Ѿ������proxy�����������ʱ��proxy�������ģ�
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

    for (std::map<Uvss::CallbackReceiverPrx, std::string>::iterator
            p = proxyEndpointMap_.begin(); p != proxyEndpointMap_.end(); ++p) {
        if (p->second == endpoint) {
            try {
                IceUtil::Handle<Callback> cb = new Callback;
                p->first->begin_sendData(strings, fileNames, files,
                    Uvss::newCallback_CallbackReceiver_sendData(cb,
                    &Callback::response, &Callback::exception));
                break;
            }
            catch (const Ice::Exception& ex) {
                std::cerr << "sendCheckInfo:\n" << ex << std::endl;
//             ���ڴ˴�ɾ��ʧЧproxy
//             ֻ�������̼߳��Զ����Ӻ�ɾ��ʧЧproxy
//             ���ڴ˴�ɾ��ʧЧproxy
//             1.���ڴ˴��ص�֪ͨ�������߳�Ҳ�����ٻص�֪ͨһ�Σ��ڴ˴�ɾ��ʧЧproxy���������̻߳�û�м��proxy������
//             2.�����ڴ˴��ص�֪ͨ�������߳̿��ܻ�©��֪ͨ���ڴ˴�ɾ��ʧЧproxy���������߳��Ѿ������proxy�����������ʱ��proxy�������ģ�
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
    if(heartbeatThread_.joinable()) {
        heartbeatThread_.join();
    }
}

#endif
