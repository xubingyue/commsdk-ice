#ifndef PEERPROXIES_H
#define PEERPROXIES_H

#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>

#include <callback.h>
#include <uvssclientwrapper.h>

typedef UVSSMessageCallback ConnectionCallback;

class PeerProxies
{
public:
    PeerProxies();
    void run();
    void start();
    int add(const std::shared_ptr<Uvss::CallbackSenderPrx>&, const std::string&);
    bool findAndRemove(int, std::string&, std::shared_ptr<Uvss::CallbackSenderPrx>&);
    bool isRepeated(const std::string&);
    int serverConnectionId(const std::string&);

    void destroy();
    void join();

    static void setConnectionCallback(ConnectionCallback);

private:
    std::map<std::shared_ptr<Uvss::CallbackSenderPrx>, std::string> serverEndpointMap_;
    std::map<std::string, int> endpointConnectionIdMap_;
    int connectionId_;
    bool destroy_;

    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread senderThread_;

    static ConnectionCallback connectionCallback_;
};

#endif // PEERPROXIES_H
