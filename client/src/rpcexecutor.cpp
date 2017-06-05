#include <rpcexecutor.h>
#include <boost/lexical_cast.hpp>
#include <Ice/Ice.h>

UVSSConnectionCallback RpcExecutor::connectionCallback = 0;

RpcExecutor::RpcExecutor() : index(0), isDestroyed(false)
{
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
                // 每次循环都应保持一致，无论this->serverProxyToEndpoint是否为空！
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
                        // 当destroy时，采取的方式：没有再删除失效的server连接、使用回调，
                        // 否则，以前C# UI退出时会有问题，
                        // 这种做法并不完美，若要修改仍需测试
                        return;
                    }
                    else {
                        auto serverProxy = p.first;
                        std::string endpoint = p.second;
                        
                        //std::cout << "1: " << endpoint << std::endl;//ok
                        //std::cout << "2: " << serverProxy->ice_getConnection()->getEndpoint()->toString() << std::endl;//wrong!代理已经失效
                        
                        // lock
                        int index = this->endpointToIndex[endpoint];
                        this->serverProxyToEndpoint.erase(serverProxy);
                        this->endpointToIndex.erase(endpoint);
                        // unlock
                        
                        std::string message("服务器端 " + endpoint + ": " +
                                "已断开 | 连接标识: " + boost::lexical_cast<std::string>(index));
                        connectionCallback(index, -3, message.c_str());
                    }
                }
            }
        }
    }
}

// 连接成功后 add!
int RpcExecutor::add(const std::shared_ptr<UVSS::ServerPrx>& server, const std::string& endpoint)
{
    std::unique_lock<std::mutex> lock(_mutex);

    ++index;
    endpointToIndex[endpoint] = index;
    serverProxyToEndpoint[server] = endpoint;
    return index;
}

// 断开连接后 remove!
bool RpcExecutor::findAndRemove(int index, std::string& endpoint)
{
    //锁的方式需要更细致！
    std::unique_lock<std::mutex> lock(_mutex);

    for (auto x : endpointToIndex) {
        if (x.second == index) {
            /*std::string */endpoint = x.first;
            endpointToIndex.erase(endpoint);
            for (auto y : serverProxyToEndpoint) {
                if (y.second == endpoint) {
                    
                    //使server到client的心跳失败
                    y.first->ice_getConnection()->close(Ice::ConnectionClose::Gracefully);
                    //client不再连接server y.first
                    serverProxyToEndpoint.erase(y.first); // 必须在此处删除对端代理
                    return true;
                }
            }

            std::cerr << "error!" << std::endl;
            return false;//可以去掉 基本不可能发生
        }
    }

    return false;//没有此连接
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

void RpcExecutor::setConnectionCallback(
        UVSSConnectionCallback connectionInfoCallback)
{
    RpcExecutor::connectionCallback = connectionInfoCallback;
}
