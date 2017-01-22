#include <clienti.h>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <clientserver.h>

ClientConnectionInfoCallback ClientI::connectionInfoCallback = 0;
ClientCheckInfoCallback ClientI::checkInfoCallback = 0;

ClientI::ClientI() : index(0), isDestroyed(false)
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

void ClientI::writeCheckInfo(
        const std::string& uVSSImageName, const UVSS::ByteSeq& uVSSImage,
        const std::string& plateImageName, const UVSS::ByteSeq& plateImage,
        const std::string& channel, const std::string& plateNumber,
        const std::string& direction, const std::string& time,
        const std::string& extension, const Ice::Current& curr)
{
    createImageDirectory("UVSS");

    
    boost::filesystem::path currentPath =
            boost::filesystem::current_path();
    std::string imagePath = currentPath.string() + "/UVSS/";

    std::string uVSSImagePath;
    if (!uVSSImageName.empty()) {
        uVSSImagePath = imagePath + uVSSImageName;
        std::ofstream ofs(uVSSImagePath, std::ios::binary);
        ofs.write((char*)&uVSSImage[0], uVSSImage.size());
    }

    std::string plateImagePath;
    if (!plateImageName.empty()) {
        plateImagePath = imagePath + plateImageName;
        std::ofstream ofs(plateImagePath, std::ios::binary);
        ofs.write((char*)&plateImage[0], plateImage.size());
    }

    if (this->checkInfoCallback != 0) {
        if (curr.con != 0) {
            //std::cout << curr.con->getEndpoint()->toString();
            Ice::ConnectionInfoPtr info = curr.con->getInfo();
            Ice::TCPConnectionInfoPtr tcpInfo =
                    Ice::TCPConnectionInfoPtr::dynamicCast(info);
            if (tcpInfo != 0) {
                std::string endpoint = tcpInfo->remoteAddress + ":" +
                        boost::lexical_cast<std::string>(tcpInfo->remotePort);
                int index = this->endpointToIndex[endpoint];

                this->checkInfoCallback(index,
                        uVSSImagePath.c_str(), plateImagePath.c_str(),
                        channel.c_str(), plateNumber.c_str(), direction.c_str(),
                        time.c_str(), extension.c_str());
            }
        }
    }
}

// void ClientI::run()
void ClientI::start()
{
    std::thread t([this]() {
    while (true) {
        std::map<UVSS::ServerPrx, std::string> serverProxyToEndpoint;

        {
//             IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
//             IceUtil::Monitor<IceUtil::Mutex>::timedWait(
//                     IceUtil::Time::seconds(2));
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
            for (std::map<UVSS::ServerPrx, std::string>::const_iterator
                    it = serverProxyToEndpoint.begin();
                    it != serverProxyToEndpoint.end(); ++it) {
                try {
                    it->first->ice_ping();
                }
                catch (...) {
//                     IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
                    std::unique_lock<std::mutex> lock(_mutex);

                    if (this->isDestroyed) {
                        return;
                    }
                    else {
                        UVSS::ServerPrx serverProxy = it->first;
                        std::string endpoint = it->second;
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
    _senderThread = std::move(t);
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
//         IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
        std::unique_lock<std::mutex> lock(_mutex);
        this->isDestroyed = true;
        _cv.notify_one();

//         IceUtil::Monitor<IceUtil::Mutex>::notify();
    }

//     IceUtil::Thread::getThreadControl().join();
    _senderThread.join();
}
