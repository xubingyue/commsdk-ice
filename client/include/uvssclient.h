#ifndef UVSSCLIENT_H
#define UVSSCLIENT_H

#include <string>
#include <Ice/Ice.h>
#include <clienti.h>

class UVSSClient : public IceUtil::Shared {
public:
    UVSSClient();

    void setConnectionInfoCallback(ClientConnectionInfoCallback);
    void setCheckInfoCallback(ClientCheckInfoCallback);
    int init();
    void uninit();
    int connect(const std::string& = "127.0.0.1", int = 20145);
    int disconnect(int);

private:
    Ice::CommunicatorPtr ic;
    Ice::ObjectAdapterPtr adapter;
    Ice::Identity id;
    std::shared_ptr<ClientI> client;
};

#endif
