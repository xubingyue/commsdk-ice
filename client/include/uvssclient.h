#ifndef UVSSCLIENT_H
#define UVSSCLIENT_H

#include <string>
#include <Ice/Ice.h>
#include <clienti.h>


class UVSSClient {
public:
    UVSSClient();

    void setConnectionInfoCallback(UVSSMessageCallback);
    void setCheckInfoCallback(UVSSCheckInfoCallback);
    int init();
    void uninit();
    int connect(const std::string& = "127.0.0.1", int = 20145);
    int disconnect(int);

private:
    Ice::CommunicatorPtr ic;
    Ice::ObjectAdapterPtr adapter;
    Ice::Identity id;
    std::shared_ptr<ClientI> client;
    
    UVSSCheckInfoCallback checkInfoCallback;
};

#endif
