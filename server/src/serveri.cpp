#include <serveri.h>

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <memory>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include <clientserver.h>
#include <version.h>

UVSSServerCallback ServerI::connectionInfoCallback = 0;

ServerI::ServerI() : isDestroyed(false)
{
}

void ServerI::setConnectionInfoCallback(
        UVSSServerCallback connectionInfoCallback)
{
    ServerI::connectionInfoCallback = connectionInfoCallback;
}

bool ServerI::checkVersion(std::string ver, const Ice::Current&)
{
    return ver == UVSS_COMM_SDK_VER;
}

void ServerI::addClient(Ice::Identity id, const Ice::Current& curr)
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

void ServerI::start()
{
    std::thread t([this]() {
        while (true) {
            std::map<std::shared_ptr<UVSS::ClientPrx>, std::string> clientProxyToEndpoint;
            
            {
                std::unique_lock<std::mutex> lock(this->_mutex);
                this->_cv.wait_for(lock, std::chrono::seconds(2));

                if (this->isDestroyed) {
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

                        if (this->isDestroyed) {
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
    });
    _senderThread = std::move(t);
}

void ServerI::filePathToBinary(const std::string& filePath, UVSS::ByteSeq& file)
{
    std::ifstream ifs(filePath, std::ios::binary);
    ifs.seekg(0, std::ios::end);
    std::streampos fileSize = ifs.tellg();
    
    file.resize(fileSize);
    ifs.seekg(0, std::ios::beg);
    ifs.read((char*)&file[0], fileSize);
}

const std::string ServerI::createCurrentTime()
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

void ServerI::sendCheckInfo(
        const std::string& uVSSImagePath, const std::string& plateImagePath,
        const std::string& channel, const std::string& plateNumber,
        const std::string& direction, const std::string& time,
        const std::string& extension)
{
    std::string timeName = createCurrentTime();

    boost::filesystem::path uVSSPath(uVSSImagePath);
    std::string uVSSImageName;
    UVSS::ByteSeq uVSSImage;
    if (boost::filesystem::exists(uVSSPath)) {
        uVSSImageName = "UVSS_" + timeName + ".jpg";
        filePathToBinary(uVSSImagePath, uVSSImage);
    }

    boost::filesystem::path platePath(plateImagePath);
    std::string plateImageName;
    UVSS::ByteSeq plateImage;
    if (boost::filesystem::exists(platePath)) {
        plateImageName = "ANPR_" + timeName + ".jpg";
        filePathToBinary(plateImagePath, plateImage);
    }

    std::unique_lock<std::mutex> lock(_mutex);
    
    for (auto p : this->clientProxyToEndpoint) {
    try {
//             p.first->writeCheckInfo(
//                 uVSSImageName, uVSSImage, plateImageName, plateImage,
//                 channel, plateNumber, direction, time, extension);

        p.first->writeCheckInfoAsync(
            uVSSImageName, uVSSImage, plateImageName, plateImage,
            channel, plateNumber, direction, time, extension,
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

void ServerI::destroy()
{
    {
        std::unique_lock<std::mutex> lock(_mutex);
        this->isDestroyed = true;
        _cv.notify_one();
    }

    _senderThread.join();
}
