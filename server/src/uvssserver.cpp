#include <uvssserver.h>
#include <boost/lexical_cast.hpp>
#include <serveri.h>

UVSSServer::UVSSServer() : port(20145)
{
}

void UVSSServer::setConnectionInfoCallback(
        UVSSServerCallback connectionInfoCallback)
{
    ServerI::setConnectionInfoCallback(connectionInfoCallback);
}

void UVSSServer::setPort(int port)
{
    this->port = port;
}

int UVSSServer::init()
{
    try {
        this->server = std::make_shared<ServerI>();

        Ice::PropertiesPtr props = Ice::createProperties();
        props->setProperty("Ice.Warn.Connections", "1");//-
        //props->setProperty("Ice.Default.Host", "localhost");//-只能localhost
        //props->setProperty("Ice.MessageSizeMax", "51200");
        props->setProperty("Ice.MessageSizeMax", "2097152");
        Ice::InitializationData initData;
        initData.properties = props;
        this->ic = Ice::initialize(initData);

        //以上代码考虑写在构造函数中
        
        Ice::ObjectAdapterPtr adapter =
                this->ic->createObjectAdapterWithEndpoints(
               "UVSS.Server", "tcp -p " + boost::lexical_cast<std::string>(this->port));
        adapter->add(this->server, Ice::stringToIdentity("Server"));
        
        adapter->activate();
        this->server->start();//启动心跳线程
    }
    catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        //no callback

        return -1;
    }

    return 0;
}

//unint有问题！//使用时，没有warning
void UVSSServer::uninit() //写在析构函数里？按理应该有uninit功能，析构函数写成空？
{
    if (this->server != 0) {
        try {
            this->server->destroy();
        }
        catch (const Ice::Exception& ex) {
            std::cerr << ex << std::endl;
        }
    }
    
    //加上adapter->deactivate();？！！！

   if (this->ic != 0) { //写在析构函数里？ 用Holder？
        try {
            this->ic->destroy();
        }
        catch (const Ice::Exception& ex) {
            std::cerr << ex << std::endl;
        }
    }
}

void UVSSServer::sendCheckInfo(
        const std::vector<std::string>& path,
        const std::vector<std::string>& v)
{
    this->server->sendCheckInfo(path, v);
}
