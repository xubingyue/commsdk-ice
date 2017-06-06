#include <uvssserver.h>

#include <chrono>
#include <ctime>
#include <iomanip>

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
    props->setProperty("Ice.MessageSizeMax", "2097152"); // 51200
    Ice::InitializationData initData;
    initData.properties = props;

    ic_ = Ice::initialize(initData);
    adapter_ = ic_->createObjectAdapterWithEndpoints("UvssServerAdapter",
        "tcp -p " + boost::lexical_cast<std::string>(port_));
}

UvssServer::~UvssServer()
{
    ic_->destroy();
    peerProxies_->join();
}

int UvssServer::start()
{
    try {
        adapter_->add(sender_, Ice::stringToIdentity("UvssServer"));
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

const std::string UvssServer::createCurrentTime()
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

void UvssServer::sendCheckInfo(const std::vector<std::string>& filePaths,
                               const std::vector<std::string>& strings)
{
    std::string time = createCurrentTime();

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

    peerProxies_->sendCheckInfo(fileNames, files, strings);
}

// 使用时，没有warning?
void UvssServer::shutdown()
{
    peerProxies_->destroy();
    ic_->shutdown();
}

void UvssServer::setConnectionCallback(ConnectionCallback connectionCallback)
{
    PeerProxies::setConnectionCallback(connectionCallback);
}

void UvssServer::setPort(int port)
{
    port_ = port;
}
