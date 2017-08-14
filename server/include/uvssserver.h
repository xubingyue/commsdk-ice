#ifndef UVSSSERVER_H
#define UVSSSERVER_H

#include <Ice/Ice.h>

#include <rpcproxies.h>
#include <workqueue.h>
#include <callbacksenderi.h>

#ifdef ICE_CPP11_MAPPING

class UvssServer {
public:
    static void setPort(int);

    UvssServer();
    int start();

    void sendCheckInfo(const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&);
    void sendCheckInfo(const std::string&,
                       const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&);
    void sendCheckInfo(const std::string&, const std::string&);
    void sendCheckInfo(const std::string&,
                       const std::string&, const std::string&);

    void shutdown();
    ~UvssServer();

private:
    Ice::CommunicatorPtr ic_;
    Ice::ObjectAdapterPtr adapter_;
    Ice::Identity ident_;

    // Ϊ���ݾ�C�ӿڣ�init�����е���C++���캯����start��initǰ����setPort�����˴�port��Ϊstatic������Ϊȫ�ֱ����õ㣩
    static int port_;

    std::shared_ptr<WorkQueue> queue_;
    std::shared_ptr<RpcProxies> proxies_;
    std::shared_ptr<CallbackSenderI> servant_;

    void filePathToFile(const std::string&, std::vector<unsigned char>&);
    void filePathsToFileNamesAndFiles(const std::vector<std::string>&,
                                      std::vector<std::string>&,
                                      std::vector<std::vector<unsigned char>>&,
                                      bool);
    void transformCheckInfo(const std::string&, const std::string&,
                            const std::string&, const std::string&,
                            const std::string&, const std::string&,
                            const std::string&,
                            std::vector<std::string>&,
                            std::vector<std::string>&,
                            std::vector<std::vector<unsigned char>>&);
    void transformCheckInfo(const std::string&, const std::string&,
                            std::vector<std::string>&,
                            std::vector<std::string>&,
                            std::vector<std::vector<unsigned char>>&);
};

#else

class UvssServer {
public:
    static void setPort(int);

    UvssServer();
    int start();

    void sendCheckInfo(const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&);
    void sendCheckInfo(const std::string&,
                       const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&);
    void sendCheckInfo(const std::string&, const std::string&);
    void sendCheckInfo(const std::string&,
                       const std::string&, const std::string&);

    void shutdown();
    ~UvssServer();

private:
    Ice::CommunicatorPtr ic_;
    Ice::ObjectAdapterPtr adapter_;
    Ice::Identity ident_;

    // Ϊ���ݾ�C�ӿڣ�init�����е���C++���캯����start��initǰ����setPort�����˴�port��Ϊstatic������Ϊȫ�ֱ����õ㣩
    static int port_;

    IceUtil::Handle<WorkQueue> queue_;
    IceUtil::Handle<RpcProxies> proxies_;
    IceUtil::Handle<CallbackSenderI> servant_;

    void filePathToFile(const std::string&, std::vector<unsigned char>&);
    void filePathsToFileNamesAndFiles(const std::vector<std::string>&,
                                      std::vector<std::string>&,
                                      std::vector<std::vector<unsigned char> >&,
                                      bool);
    void transformCheckInfo(const std::string&, const std::string&,
                            const std::string&, const std::string&,
                            const std::string&, const std::string&,
                            const std::string&,
                            std::vector<std::string>&,
                            std::vector<std::string>&,
                            std::vector<std::vector<unsigned char> >&);
    void transformCheckInfo(const std::string&, const std::string&,
                            std::vector<std::string>&,
                            std::vector<std::string>&,
                            std::vector<std::vector<unsigned char> >&);
};

#endif

#endif
