#include "rpcexecutor.h"
#include <boost/lexical_cast.hpp>
#include <Ice/Ice.h>

UVSSMessageCallback RpcExecutor::connectionInfoCallback = 0;

RpcExecutor::RpcExecutor() : index(0), isDestroyed(false)
{
}

void RpcExecutor::setConnectionInfoCallback(
        UVSSMessageCallback connectionInfoCallback)
{
    RpcExecutor::connectionInfoCallback = connectionInfoCallback;
}

void RpcExecutor::useConnectionInfoCallback(int index, int type,
        const std::string& connectionInfo)
{
    if (this->connectionInfoCallback != 0) {
        this->connectionInfoCallback(index, type, connectionInfo.c_str());
    }
}

void RpcExecutor::start()
{
    std::thread t([this]() {
        this->run();
    });
    _receiverThread = std::move(t);
}

void RpcExecutor::join()
{
    if (_receiverThread.joinable()) {
        _receiverThread.join();
    }
}

void RpcExecutor::run()
{
    while (true) {
        std::map<std::shared_ptr<UVSS::ServerPrx>, std::string> serverProxyToEndpoint;

        {
            std::unique_lock<std::mutex> lock(this->_mutex);
            this->_cv.wait_for(lock, std::chrono::seconds(2));

            if (this->isDestroyed) {
                return;
            }
            else {
                serverProxyToEndpoint = this->serverProxyToEndpoint;
            }
        }

        if (!serverProxyToEndpoint.empty()) {
            for (auto p : serverProxyToEndpoint) {
                try {
                    p.first->ice_ping();
                }
                catch (...) {
                    std::unique_lock<std::mutex> lock(_mutex);

                    if (this->isDestroyed) {
                        return;
                    }
                    else {
                        auto serverProxy = p.first;
                        std::string endpoint = p.second;
                        
                        //std::cout << "1: " << endpoint << std::endl;//ok
                        //std::cout << "2: " << serverProxy->ice_getConnection()->getEndpoint()->toString() << std::endl;//wrong!
                        
                        int index = this->endpointToIndex[endpoint];

                        useConnectionInfoCallback(index, -3,
                                "服务器端 " + endpoint + ": " +
                                "已断开 | 连接标识: " + boost::lexical_cast<std::string>(index));
                        this->serverProxyToEndpoint.erase(serverProxy);
                        this->endpointToIndex.erase(endpoint);
                    }
                }
            }
        }
    }    
}

int RpcExecutor::connect(const std::shared_ptr<UVSS::ServerPrx>& server, const std::string& endpoint)
{
        //锁的方式需要更细致！
    std::unique_lock<std::mutex> lock(_mutex);

    try {
        ++index;
        endpointToIndex[endpoint] = index;
        serverProxyToEndpoint[server] = endpoint;

        useConnectionInfoCallback(index, 1,
                "服务器端 " + endpoint + ": " + "已连接 | 连接标识: " + boost::lexical_cast<std::string>(index));
    }
    catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        useConnectionInfoCallback(-1, -2, "连接失败");

        return -1;
    }
    catch (const char* msg) {
        std::cerr << msg << std::endl;
        useConnectionInfoCallback(-1, -2, "连接失败");

        return -1;
    }

    return index;
}

int RpcExecutor::disconnect(int index)
{
        //锁的方式需要更细致！
    std::unique_lock<std::mutex> lock(_mutex);

    try {
        for (auto x : endpointToIndex) {
            if (x.second == index) {
                std::string endpoint = x.first;
                endpointToIndex.erase(endpoint);
                for (auto y : serverProxyToEndpoint) {
                    if (y.second == endpoint) {
                        //使server到client的心跳失败
                        y.first->ice_getConnection()->close(Ice::ConnectionClose::Gracefully);
                        //client不再连接server y.first
                        serverProxyToEndpoint.erase(y.first);//无须it2++

                        //只能在此处通知！不能依靠心跳线程
                        useConnectionInfoCallback(index, -3,
                                                                "服务器端 " + endpoint + ": " +
                                                                "已断开 | 连接标识: " + boost::lexical_cast<std::string>(index));

                        return 1;
                    }
                }

                return -1;//可以去掉
            }
        }

        return -1;//没有此连接
    }
    catch (...) {
        return -1;//断开失败, 以前的程序没有考虑
    }
}

void RpcExecutor::destroy()
{    
    std::unique_lock<std::mutex> lock(_mutex);
    this->isDestroyed = true;
    _cv.notify_one();
}

bool RpcExecutor::isRepeated(const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(_mutex);
    for (auto p : serverProxyToEndpoint) {
            if (p.second == endpoint) {
                return true;
            }
    }
    return false;
}

int RpcExecutor::serverIndex(const Ice::Current& curr)
{
    std::unique_lock<std::mutex> lock(_mutex);
    if (curr.con != 0) {
        Ice::ConnectionInfoPtr info = curr.con->getInfo();
        Ice::TCPConnectionInfoPtr tcpInfo =
            std::dynamic_pointer_cast<Ice::TCPConnectionInfo>(info);
        if (tcpInfo != 0) {
    //std::cout << curr.con->getEndpoint()->toString() << std::endl;
     //tcp -h 127.0.0.1 -p 20145 -t 60000
            
            // 考虑简化下面代码
            std::string endpoint = tcpInfo->remoteAddress + ":" +
                                   boost::lexical_cast<std::string>(tcpInfo->remotePort);
//             std::cout << endpoint << std::endl;
            
            return this->endpointToIndex[endpoint];
        }
    }
    
    return -1; //new
}
