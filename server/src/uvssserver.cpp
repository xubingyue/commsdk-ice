#include <uvssserver.h>

#include <chrono>
#include <ctime>
#include <iomanip>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <callbacksenderi.h>

int UvssServer::port_ = 20145;

UvssServer::UvssServer() :
    rpcProxies_(std::make_shared<RpcProxies>()),
    sender_(std::make_shared<CallbackSenderI>(rpcProxies_))
{
//     try...catch?
    Ice::PropertiesPtr props = Ice::createProperties();
//     props->setProperty("Ice.Default.Host", "localhost"); // 这样只能localhost
    props->setProperty("Ice.Warn.Connections", "1");
    props->setProperty("Ice.MessageSizeMax", "0");
    Ice::InitializationData initData;
    initData.properties = props;

    ich_ = Ice::initialize(initData);
    adapter_ = ich_->createObjectAdapterWithEndpoints("UvssServerAdapter",
        "tcp -p " + boost::lexical_cast<std::string>(port_));
    ident_ = Ice::stringToIdentity("UvssServer");
}

UvssServer::~UvssServer()
{
    rpcProxies_->join();
}

int UvssServer::start()
{
    try {
        adapter_->add(sender_, ident_);
        adapter_->activate();
        rpcProxies_->start(); // 启动心跳线程
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

void UvssServer::sendCheckInfo(const std::string& uvssImagePath,
                               const std::string& plateImagePath,
                               const std::string& channel,
                               const std::string& plateNumber,
                               const std::string& direction,
                               const std::string& dateTime,
                               const std::string& extension)
{
    std::vector<std::string> strings;
    strings.push_back(channel);
    strings.push_back(plateNumber);
    strings.push_back(direction);
    strings.push_back(dateTime);
    strings.push_back(extension);

    std::string time = boost::posix_time::to_iso_string(
                           boost::posix_time::microsec_clock::local_time());

    std::string uvssImageName;
    std::vector<unsigned char> uvssImage;
    boost::filesystem::path filePath(uvssImagePath);
    if (boost::filesystem::exists(filePath)) {
        uvssImageName = time + "_UVSS" + filePath.extension().string();
        filePathToFile(uvssImagePath, uvssImage);
    }

    std::string plateImageName;
    std::vector<unsigned char> plateImage;
    boost::filesystem::path filePath1(plateImagePath);
    if (boost::filesystem::exists(filePath1)) {
        plateImageName = time + "_ANPR" + filePath1.extension().string();
        filePathToFile(plateImagePath, plateImage);
    }

    std::vector<std::string> fileNames;
    std::vector<std::vector<unsigned char>> files;

    fileNames.push_back(uvssImageName);
    fileNames.push_back(plateImageName);
    files.push_back(uvssImage);
    files.push_back(plateImage);

    rpcProxies_->sendCheckInfo(strings, fileNames, files);
}

void UvssServer::sendCheckInfo(const std::string& concatedString,
                               const std::string& concatedFilePath)
{
    std::vector<std::string> strings;
    strings.push_back(std::string(concatedString));

    std::vector<std::string> filePaths;
    const char* concatedFilePathC = concatedFilePath.c_str();
    boost::split(filePaths, concatedFilePathC, boost::is_any_of("|"),
                 boost::token_compress_on);

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

    rpcProxies_->sendCheckInfo(strings, fileNames, files);
}

// 使用时，没有warning?
void UvssServer::shutdown()
{
    rpcProxies_->destroy();
    ich_->shutdown();
}

void UvssServer::setPort(int port)
{
    port_ = port;
}
