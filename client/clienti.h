#ifndef CLIENTI_H
#define CLIENTI_H

#include <map>
#include <string>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <clientserver.h>

class ClientI;
typedef IceUtil::Handle<ClientI> ClientIPtr;
typedef void (__stdcall *ClientConnectionInfoCallback)(int, int, const char*);
typedef void (__stdcall *ClientCheckInfoCallback)(int, const char*, const char*,
        const char*, const char*, const char*, const char*, const char*);

class ClientI : virtual public UVSS::Client, virtual public IceUtil::Thread,
        virtual public IceUtil::Monitor<IceUtil::Mutex> {
public:
    ClientI();

    static void setConnectionInfoCallback(ClientConnectionInfoCallback);
    static void setCheckInfoCallback(ClientCheckInfoCallback);

    virtual void writeCheckInfo(
            const std::string&, const UVSS::ByteSeq&,
            const std::string&, const UVSS::ByteSeq&,
            const std::string&, const std::string&, const std::string&,
            const std::string&, const std::string&,
            const Ice::Current& = Ice::Current());
    virtual void run();

    void useConnectionInfoCallback(int, int, const std::string&);//¿¼ÂÇÉ¾³ý´Ëº¯Êý
    void createImageDirectory(const std::string&);
    void destroy();

    int index;
    std::map<UVSS::ServerPrx, std::string> serverProxyToEndpoint;
    std::map<std::string, int> endpointToIndex;

private:
    static ClientConnectionInfoCallback connectionInfoCallback;
    static ClientCheckInfoCallback checkInfoCallback;

    bool isDestroyed;
};

#endif
