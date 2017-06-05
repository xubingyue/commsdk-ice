#include <uvssserver.h>

#include <chrono>
#include <ctime>
#include <iomanip>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include <serveri.h>

int UVSSServer::port = 20145;

void UVSSServer::setConnectionInfoCallback(
    UVSSServerCallback connectionInfoCallback)
{
    RpcExecutor::setConnectionInfoCallback(connectionInfoCallback);
}

void UVSSServer::setPort(int port)
{
    UVSSServer::port = port;
}

UVSSServer::UVSSServer()
{
    _workQueue = std::make_shared<RpcExecutor>();
    this->server = std::make_shared<ServerI>(_workQueue);

    Ice::PropertiesPtr props = Ice::createProperties();
    props->setProperty("Ice.Warn.Connections", "1");//-
    //props->setProperty("Ice.Default.Host", "localhost");//-只能localhost
    props->setProperty("Ice.MessageSizeMax", "2097152");// "51200
    Ice::InitializationData initData;
    initData.properties = props;
    this->ic = Ice::initialize(initData);
}

int UVSSServer::init()
{
    try {
        Ice::ObjectAdapterPtr adapter =
            this->ic->createObjectAdapterWithEndpoints(
                "UVSS.Server", "tcp -p " + boost::lexical_cast<std::string>(this->port));
        adapter->add(this->server, Ice::stringToIdentity("Server"));

        //
        adapter->activate();
        _workQueue->start();//启动心跳线程
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
    try {
        _workQueue->destroy();
    }
    catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    }

    //加上adapter->deactivate();？！！！

    if (this->ic != 0) { //写在析构函数里？ 用Holder？
        try {
            this->ic->destroy(); // shutdown()?
        }
        catch (const Ice::Exception& ex) {
            std::cerr << ex << std::endl;
        }
    }

    _workQueue->join();
}

void UVSSServer::sendCheckInfo(
    const std::vector<std::string>& ns,
    const std::vector<std::string>& v)
{
    std::vector<std::string> names;
    UVSS::ByteSeqSeq bss;

    std::string timeName = createCurrentTime();

    for (int i = 0; i != ns.size(); ++i) {
        boost::filesystem::path pt(ns[i]);
        UVSS::ByteSeq bs;
        if (boost::filesystem::exists(pt)) {
            names.push_back(timeName + "[" + boost::lexical_cast<std::string>(i) + "]_" + pt.filename().string());
            filePathToBinary(ns[i], bs);
            bss.push_back(bs);
        }
    }

    _workQueue->sendCheckInfo(names, bss, v);
}

void UVSSServer::filePathToBinary(const std::string& filePath, UVSS::ByteSeq& file)
{
    std::ifstream ifs(filePath, std::ios::binary);
    ifs.seekg(0, std::ios::end);
    std::streampos fileSize = ifs.tellg();

    file.resize(fileSize);
    ifs.seekg(0, std::ios::beg);
    ifs.read((char*)&file[0], fileSize);
}

const std::string UVSSServer::createCurrentTime()
{
    auto now = std::chrono::system_clock::now();

    std::time_t timer = std::chrono::system_clock::to_time_t(now);
    std::tm timeInfo = *std::localtime(&timer);

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch());
    auto s = std::chrono::duration_cast<std::chrono::seconds>(
                 now.time_since_epoch());
    auto msPart = ms - s;

    std::stringstream currentTime;
    currentTime << std::put_time(&timeInfo, "%Y%m%d%H%M%S")
                << std::setw(3) << std::setfill('0') << msPart.count();

    return currentTime.str();
}
