#ifndef RPCEXECUTOR_H
#define RPCEXECUTOR_H

#include <condition_variable>
#include <map>
#include <mutex>
#include <string>
#include <thread>

#include <memory>

#include <uvssclientsdk.h>
#include <clientserver.h>

class RpcExecutor
{
public:
    RpcExecutor();
    
    void start();
    void join();
    void run();
    void destroy();
    int connect(const std::shared_ptr<UVSS::ServerPrx>&, const std::string&);
    int disconnect(int);
    
    bool isRepeated(const std::string&);
    
    int serverIndex(const Ice::Current& curr);
    
    std::map<std::shared_ptr<UVSS::ServerPrx>, std::string> serverProxyToEndpoint;
    std::map<std::string, int> endpointToIndex;
    int index;
    bool isDestroyed;
    
    std::mutex _mutex;
    std::condition_variable _cv;
    std::thread _receiverThread;
    
    
    static void setConnectionInfoCallback(UVSSMessageCallback);                                              
    void useConnectionInfoCallback(int, int, const std::string&);//考虑删除此函数
    
private:
    static UVSSMessageCallback connectionInfoCallback;
};

#endif // RPCEXECUTOR_H
