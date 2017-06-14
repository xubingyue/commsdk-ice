#include <uvssserver.h>

#include <chrono>
#include <ctime>
#include <iomanip>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include <callbacksenderi.h>

int UvssServer::port_ = 20145;

UvssServer::UvssServer() :
    peerProxies_(std::make_shared<PeerProxies>()),
    sender_(std::make_shared<CallbackSenderI>(peerProxies_))
{
//     try...catch?
    Ice::PropertiesPtr props = Ice::createProperties();
//     props->setProperty("Ice.Default.Host", "localhost"); // 这样只能localhost
    props->setProperty("Ice.Warn.Connections", "1");
    props->setProperty("Ice.MessageSizeMax", "0");
    Ice::InitializationData initData;
    initData.properties = props;

    ic_ = Ice::initialize(initData);
    adapter_ = ic_->createObjectAdapterWithEndpoints("UvssServerAdapter",
        "tcp -p " + boost::lexical_cast<std::string>(port_));
    ident_ = Ice::stringToIdentity("UvssServer");
}

UvssServer::~UvssServer()
{
    ic_->destroy();
    peerProxies_->join();
}

int UvssServer::start()
{
    try {
        adapter_->add(sender_, ident_);
        adapter_->activate();
        peerProxies_->start(); // 启动心跳线程
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}

void UvssServer::filePathToFile(const std::string& filePath,
                                std::vector<unsigned char>& file)
{
    std::ifstream ifs(filePath, std::ios::binary);

    ifs.seekg(0, std::ios::end);
    std::streampos fileSize = ifs.tellg();
    file.resize(fileSize);

    ifs.seekg(0, std::ios::beg);
    ifs.read((char*)&file[0], fileSize);
}

void UvssServer::sendCheckInfo(const std::vector<std::string>& strings,
                               const std::vector<std::string>& filePaths)
{
    std::string time = boost::posix_time::to_iso_string(
        boost::posix_time::microsec_clock::local_time());

    std::vector<std::string> fileNames;
    std::vector<std::vector<unsigned char>> files;
    for (int i = 0; i != filePaths.size(); ++i) {
        std::string fileName;
        std::vector<unsigned char> file;

        boost::filesystem::path filePath(filePaths[i]);
        if (boost::filesystem::exists(filePath)) {
            fileName = time + "[" + boost::lexical_cast<std::string>(i) + "]_" + filePath.filename().string();
            filePathToFile(filePaths[i], file);
        }

        fileNames.push_back(fileName);
        files.push_back(file);
    }

    peerProxies_->sendCheckInfo(strings, fileNames, files);
}

// 使用时，没有warning?
void UvssServer::shutdown()
{
    peerProxies_->destroy();
    ic_->shutdown();
}

void UvssServer::setPort(int port)
{
    port_ = port;
}
