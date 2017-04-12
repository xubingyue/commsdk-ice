#include <clienti.h>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <clientserver.h>

#include <exception>

ClientConnectionInfoCallback ClientI::connectionInfoCallback = 0;
// ClientCheckInfoCallback ClientI::checkInfoCallback = 0;

ClientI::ClientI(const std::shared_ptr<WorkQueue>& workQueue) : index(0), isDestroyed(false), _workQueue(workQueue)
{
}

void ClientI::setConnectionInfoCallback(
        ClientConnectionInfoCallback connectionInfoCallback)
{
    ClientI::connectionInfoCallback = connectionInfoCallback;
}

// void ClientI::setCheckInfoCallback(ClientCheckInfoCallback checkInfoCallback)
// {
//     _workQueue->setCheckInfoCallback(checkInfoCallback);
// }

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
    if (curr.con != 0) {
        //std::cout << curr.con->getEndpoint()->toString();
        Ice::ConnectionInfoPtr info = curr.con->getInfo();
        Ice::TCPConnectionInfoPtr tcpInfo =
            std::dynamic_pointer_cast<Ice::TCPConnectionInfo>(info);
        if (tcpInfo != 0) {
            std::string endpoint = tcpInfo->remoteAddress + ":" +
                                   boost::lexical_cast<std::string>(tcpInfo->remotePort);
            int index = this->endpointToIndex[endpoint];
            _workQueue->add(uVSSImageName, uVSSImage,
                            plateImageName, plateImage,
                            channel, plateNumber,
                            direction, time,
                            extension,
                            move(response),
                            move(error),
                            index);
        }
    }
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

void ClientI::destroy()
{    
    {
        std::unique_lock<std::mutex> lock(_mutex);
        this->isDestroyed = true;
        _cv.notify_one();
    }
    
    _receiverThread.join();
}
