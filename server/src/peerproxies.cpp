#include <peerproxies.h>

#include <boost/lexical_cast.hpp>
#include <Ice/Ice.h>

UVSSServerCallback PeerProxies::connectionInfoCallback = 0;

PeerProxies::PeerProxies() : _destroy(false)
{
}

void
PeerProxies::start()
{
    std::thread t([this]()
    {
        this->run();
    });
    _senderThread = move(t);
}

void PeerProxies::join()
{
    if(_senderThread.joinable())
    {
        _senderThread.join();
    }
}

void PeerProxies::run()
{
    while (true) {
        std::map<std::shared_ptr<UVSS::CallbackReceiverPrx>, std::string> clientProxyToEndpoint;

        {
            std::unique_lock<std::mutex> lock(this->_mutex);
            this->_cv.wait_for(lock, std::chrono::seconds(2));

            if (this->_destroy) {
                return;
            }
            else {
                clientProxyToEndpoint = this->clientProxyToEndpoint;
            }
        }

        if (!clientProxyToEndpoint.empty()) {
            for (auto p : clientProxyToEndpoint) {
                try {
                    p.first->ice_ping();
                }
                catch (...) {
                    std::unique_lock<std::mutex> lock(_mutex);

                    if (this->_destroy) {
                        return;
                    }
                    else {
                        std::string endpoint = p.second;
                        if (this->connectionInfoCallback != 0) {
                            this->connectionInfoCallback(-1, std::string(
                                                             "客户端 " + endpoint + ": 已断开").c_str());
                        }
                        this->clientProxyToEndpoint.erase(p.first); //不用考虑迭代器失效的问题，这里是在成员变量里删除
                    }
                }
            }
        }
    }
}

void
PeerProxies::add(Ice::Identity id, const Ice::Current& curr)
{
    std::unique_lock<std::mutex> lock(_mutex);

    auto clientProxy =
        Ice::uncheckedCast<UVSS::CallbackReceiverPrx>(curr.con->createProxy(id));

    Ice::ConnectionInfoPtr info = curr.con->getInfo();
    Ice::TCPConnectionInfoPtr tcpInfo =
        std::dynamic_pointer_cast<Ice::TCPConnectionInfo>(info);

//     std::cout << curr.con->getEndpoint()->toString() << std::endl;
//     tcp -p 20145 -t 60000

    std::string endpoint = tcpInfo->remoteAddress.replace(0, 7, "") + ":" +
                           boost::lexical_cast<std::string>(tcpInfo->remotePort);//去掉开头的::ffff:

    this->clientProxyToEndpoint[clientProxy] = endpoint; // 是否独立

//     auto v = clientProxy->ice_getEndpoints();
//     std::cout << v.size() << std::endl;//0
//     std::cout << clientProxy->ice_getConnection()->getEndpoint()->toString() << std::endl;
//     tcp -p 20145 -t 60000

    if (this->connectionInfoCallback != 0) {
        this->connectionInfoCallback(
            0, std::string("客户端 " + endpoint + ": 已连接").c_str());
    }
}

void
PeerProxies::destroy()
{
    std::unique_lock<std::mutex> lock(_mutex);
    this->_destroy = true;
    _cv.notify_one();
}

void PeerProxies::setConnectionInfoCallback(
    UVSSServerCallback connectionInfoCallback)
{
    PeerProxies::connectionInfoCallback = connectionInfoCallback;
}

void PeerProxies::sendCheckInfo(
    const std::vector<std::string>& names,
    const UVSS::ByteSeqSeq& bss,
    const std::vector<std::string>& ss)
{
    std::unique_lock<std::mutex> lock(_mutex);

    for (auto p : this->clientProxyToEndpoint) {
        try {
            p.first->sendCheckInfoAsync(
                names,
                bss,
                ss,
                nullptr,
                [](std::exception_ptr e)
            {
                try
                {
                    std::rethrow_exception(e);
                }
                catch(const std::exception& ex)
                {
                    std::cerr << "sayHello AMI call failed:\n" << ex.what() << std::endl;
                }
            });
        }
        catch (const Ice::Exception& ex) {
//             just skip, no erase
//             如果在此处删除失效的client代理，这里需要回调通知一次，心跳线程也可能回调通知一次（当心跳线程的client副本还没有检测这个失效的代理之前，在此处删除失效代理）
//             如果在此处删除失效的client代理，而不回调通知，心跳线程可能会漏掉通知（当心跳线程的client副本已经检测完这个失效的代理之后，在此处删除失效代理）
//             只让心跳线程实现检测对端连接的任务和删除失效client代理
//             it = this->clientProxyToEndpoint.erase(it);
            std::cerr << ex << std::endl;
        }
    }
}
