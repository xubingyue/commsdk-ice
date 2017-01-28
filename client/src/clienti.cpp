#include <clienti.h>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <clientserver.h>

#include <exception>

ClientConnectionInfoCallback ClientI::connectionInfoCallback = 0;
ClientCheckInfoCallback ClientI::checkInfoCallback = 0;

ClientI::ClientI(const std::shared_ptr<WorkQueue>& workQueue) : index(0), isDestroyed(false), _workQueue(workQueue)
{
}

void ClientI::setConnectionInfoCallback(
        ClientConnectionInfoCallback connectionInfoCallback)
{
    ClientI::connectionInfoCallback = connectionInfoCallback;
}

void ClientI::setCheckInfoCallback(ClientCheckInfoCallback checkInfoCallback)
{
    ClientI::checkInfoCallback = checkInfoCallback;
}

void ClientI::writeCheckInfoAsync(
        std::string uVSSImageName, UVSS::ByteSeq uVSSImage,
        std::string plateImageName, UVSS::ByteSeq plateImage,
        std::string channel, std::string plateNumber,
        std::string direction, std::string time,
        std::string extension,
        std::function<void()> response,
        std::function<void(std::exception_ptr)> error,
        const Ice::Current& curr)
{
    std::cout << "start" << std::endl;
    
    if (curr.con != 0) {
            //std::cout << curr.con->getEndpoint()->toString();
            Ice::ConnectionInfoPtr info = curr.con->getInfo();
            Ice::TCPConnectionInfoPtr tcpInfo =
                    std::dynamic_pointer_cast<Ice::TCPConnectionInfo>(info);
            if (tcpInfo != 0) {
                std::string endpoint = tcpInfo->remoteAddress + ":" +
                        boost::lexical_cast<std::string>(tcpInfo->remotePort);

    _workQueue->add(uVSSImageName, uVSSImage,
                    plateImageName, plateImage,
                    channel, plateNumber,
                    direction, time,
                    extension,
                    move(response),
                    move(error),
                    endpoint);
            }
    }
    
//     createImageDirectory("UVSS");
// 
//     
//     boost::filesystem::path currentPath =
//             boost::filesystem::current_path();
//     std::string imagePath = currentPath.string() + "/UVSS/";
// 
//     std::string uVSSImagePath;
//     if (!uVSSImageName.empty()) {
//         uVSSImagePath = imagePath + uVSSImageName;
//         std::ofstream ofs(uVSSImagePath, std::ios::binary);
//         ofs.write((char*)&uVSSImage[0], uVSSImage.size());
//     }
// 
//     std::string plateImagePath;
//     if (!plateImageName.empty()) {
//         plateImagePath = imagePath + plateImageName;
//         std::ofstream ofs(plateImagePath, std::ios::binary);
//         ofs.write((char*)&plateImage[0], plateImage.size());
//     }
// 
//     if (this->checkInfoCallback != 0) {
//         /*if (curr.con != 0) {
//             //std::cout << curr.con->getEndpoint()->toString();
//             Ice::ConnectionInfoPtr info = curr.con->getInfo();
//             Ice::TCPConnectionInfoPtr tcpInfo =
//                     std::dynamic_pointer_cast<Ice::TCPConnectionInfo>(info);
//             if (tcpInfo != 0) {
//                 std::string endpoint = tcpInfo->remoteAddress + ":" +
//                         boost::lexical_cast<std::string>(tcpInfo->remotePort);*/
    
    
    
    
    
//                 int index = this->endpointToIndex[endpoint];
// 
//                 this->checkInfoCallback(index,
//                         uVSSImagePath.c_str(), plateImagePath.c_str(),
//                         channel.c_str(), plateNumber.c_str(), direction.c_str(),
//                         time.c_str(), extension.c_str());
//             }
//         }
//     }
}

void ClientI::start()
{
    std::thread t([this]() {
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
    });
    _receiverThread = std::move(t);
}

void ClientI::useConnectionInfoCallback(int index, int type,
        const std::string& connectionInfo)
{
    if (this->connectionInfoCallback != 0) {
        this->connectionInfoCallback(index, type, connectionInfo.c_str());
    }
}

void ClientI::createImageDirectory(const std::string& imageDirectory)
{
    boost::filesystem::path dir(imageDirectory);

    if (boost::filesystem::exists(dir)) {
        return;
    }
    else {
        boost::filesystem::create_directory(dir);
    }
}

void ClientI::destroy()
{
    {
        std::unique_lock<std::mutex> lock(_mutex);
        this->isDestroyed = true;
        _cv.notify_one();
    }

    _receiverThread.join();
}
