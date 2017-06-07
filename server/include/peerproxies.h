#ifndef PEERPROXIES_H
#define PEERPROXIES_H

#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>

#include <callback.h>
#include <uvssserverwrapper.h>

typedef UVSSServerCallback ConnectionCallback;

class PeerProxies
{
public:
    PeerProxies();
    void run();
    void start();
    void add(const Ice::Identity&, const Ice::Current&);
    void sendCheckInfo(const std::vector<std::string>&,
                       const std::vector<std::vector<unsigned char>>&,
                       const std::vector<std::string>&);
    void destroy();
    void join();

    static void setConnectionCallback(ConnectionCallback);

private:
    std::map<std::shared_ptr<Uvss::CallbackReceiverPrx>, std::string> clientEndpointMap_;
    bool destroy_;

    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread senderThread_;

    static ConnectionCallback connectionCallback_;
};

#endif // PEERPROXIES_H
