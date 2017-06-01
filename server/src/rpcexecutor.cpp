#include <rpcexecutor.h>
#include <Ice/Ice.h>

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include <chrono>
#include <ctime>
#include <iomanip>

UVSSServerCallback RpcExecutor::connectionInfoCallback = 0;

RpcExecutor::RpcExecutor() : _destroy(false)
{
}

void
RpcExecutor::start()
{
    std::thread t([this]()
        {
            this->run();
        });
    _senderThread = move(t);
}

void RpcExecutor::join()
{
    if(_senderThread.joinable())
    {
        _senderThread.join();
    }
}

void RpcExecutor::run()
{
    while (true) {
        std::map<std::shared_ptr<UVSS::ClientPrx>, std::string> clientProxyToEndpoint;
        
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
RpcExecutor::add(Ice::Identity id, const Ice::Current& curr)
{
    std::unique_lock<std::mutex> lock(_mutex);
    
    auto clientProxy =
            Ice::uncheckedCast<UVSS::ClientPrx>(curr.con->createProxy(id));

    Ice::ConnectionInfoPtr info = curr.con->getInfo();
    Ice::TCPConnectionInfoPtr tcpInfo = 
            std::dynamic_pointer_cast<Ice::TCPConnectionInfo>(info);

    //std::cout << curr.con->getEndpoint()->toString() << std::endl;
    //tcp -p 20145 -t 60000
            
    std::string endpoint = tcpInfo->remoteAddress.replace(0, 7, "") + ":" +
    boost::lexical_cast<std::string>(tcpInfo->remotePort);//去掉开头的::ffff:

     this->clientProxyToEndpoint[clientProxy] = endpoint; // 是否独立
//     auto v = clientProxy->ice_getEndpoints();
//     std::cout << v.size() << std::endl;//0
//std::cout << clientProxy->ice_getConnection()->getEndpoint()->toString() << std::endl;
//tcp -p 20145 -t 60000
    if (this->connectionInfoCallback != 0) {
        this->connectionInfoCallback(
                0, std::string("客户端 " + endpoint + ": 已连接").c_str());
    }
}

void
RpcExecutor::destroy()
{
    std::unique_lock<std::mutex> lock(_mutex);
    this->_destroy = true;
    _cv.notify_one();
}

void RpcExecutor::setConnectionInfoCallback(
        UVSSServerCallback connectionInfoCallback)
{
    RpcExecutor::connectionInfoCallback = connectionInfoCallback;
}

void RpcExecutor::filePathToBinary(const std::string& filePath, UVSS::ByteSeq& file)
{
    std::ifstream ifs(filePath, std::ios::binary);
    ifs.seekg(0, std::ios::end);
    std::streampos fileSize = ifs.tellg();
    
    file.resize(fileSize);
    ifs.seekg(0, std::ios::beg);
    ifs.read((char*)&file[0], fileSize);
}

const std::string RpcExecutor::createCurrentTime()
{
    auto now = std::chrono::system_clock::now();

    std::time_t timer = std::chrono::system_clock::to_time_t(now);
    std::tm timeInfo = *std::localtime(&timer);

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch());
    auto s = std::chrono::duration_cast<std::chrono::seconds>(
            now.time_since_epoch());
    auto msPart = ms - s;

    std::stringstream currentTime;
    currentTime << std::put_time(&timeInfo, "%Y%m%d%H%M%S")
            << std::setw(3) << std::setfill('0') << msPart.count();

    return currentTime.str();
}

void RpcExecutor::sendCheckInfo(
        const std::vector<std::string>& ns,
        const std::vector<std::string>& ss)
{
    std::vector<std::string> names;
    UVSS::ByteSeqSeq bss;
    
    std::string timeName = createCurrentTime();
    
    for (int i = 0; i != ns.size(); ++i) {
        boost::filesystem::path pt(ns[i]);
        UVSS::ByteSeq bs;
        if (boost::filesystem::exists(pt)) {
            names.push_back(timeName + "[" + boost::lexical_cast<std::string>(i) + "]_" + pt.filename().string());
            filePathToBinary(ns[i], bs);
            bss.push_back(bs);
        }
    }

    
    /////////////////////
    
    std::unique_lock<std::mutex> lock(_mutex);
    
    for (auto p : this->clientProxyToEndpoint) {
    try {
        p.first->writeCheckInfoAsync(
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
            ///just skip, no erase
            //如果在此处删除失效的client代理，这里需要回调通知一次，心跳线程也可能回调通知一次（当心跳线程的client副本还没有检测这个失效的代理之前，在此处删除失效代理）
            //如果在此处删除失效的client代理，而不回调通知，心跳线程可能会漏掉通知（当心跳线程的client副本已经检测完这个失效的代理之后，在此处删除失效代理）
            //只让心跳线程实现检测对端连接的任务和删除失效client代理
            //it = this->clientProxyToEndpoint.erase(it);
            std::cerr << ex << std::endl;
        }
    }
}
