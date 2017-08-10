#include <uvssclient.h>

#include <fstream>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <Ice/Ice.h>

#include <global.h>

#ifdef ICE_CPP11_MAPPING

UvssClient::UvssClient() :
    queue_(std::make_shared<WorkQueue>()),
    proxies_(std::make_shared<RpcProxies>()),
    servant_(std::make_shared<CallbackReceiverI>(queue_, proxies_))
{
//     try...catch?
    Ice::PropertiesPtr props = Ice::createProperties();
    props->setProperty("Ice.Warn.Connections", "1");
    props->setProperty("Ice.MessageSizeMax", "0");
//     props->setProperty("Ice.ACM.Client", "0");
    Ice::InitializationData initData;
    initData.properties = props;

    ic_ = Ice::initialize(initData);
    adapter_ = ic_->createObjectAdapter("");
    ident_.name = Ice::generateUUID();
    ident_.category = "";
}

int UvssClient::start()
{
    try {
        adapter_->add(servant_, ident_);
        adapter_->activate();
        queue_->start();
        proxies_->startHeartbeat();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        g_initializationCallback(-1, -1, "Initialization Failed");

        return -1;
    }

    return 1;
}

int UvssClient::connect(const std::string& ipAddress, int port)
{
    try {
        std::string endpoint(ipAddress + ":" +
            boost::lexical_cast<std::string>(port));
        if (proxies_->has(endpoint)) {
            return -2;
        }

        auto base = ic_->stringToProxy("UvssServer:tcp -h " +
            ipAddress + " -p " + boost::lexical_cast<std::string>(port));
        auto proxy = Ice::checkedCast<Uvss::CallbackSenderPrx>(base);
        if (!proxy) {
            throw std::runtime_error("Invalid proxy");
        }

//         std::cout << proxy->ice_getConnection()->getEndpoint()->toString() << std::endl;
//         std::cout << endpoint << std::endl;
//         tcp -h 192.168.1.9 -p 20145 -t 60000
//         192.168.1.9:20145

//         保留检查对端版本的功能
        if (!proxy->checkVersion(Uvss::version)) {
            return -3;
        }

        proxy->ice_getConnection()->setAdapter(adapter_);
        proxy->addProxy(ident_);

        int connectionId = proxies_->add(proxy, endpoint);

        std::string message("Server " + endpoint + ": " +
            "Connected | Connection Id: " +
            boost::lexical_cast<std::string>(connectionId));
        g_connectionCallback(connectionId, 1, message.c_str());

        return connectionId;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        g_connectionCallback(-1, -2, "Connection Failed");

        return -1;
    }
}

int UvssClient::disconnect(int connectionId)
{
    std::string endpoint;
    std::shared_ptr<Uvss::CallbackSenderPrx> proxy;
    bool ok = proxies_->remove(connectionId, endpoint, proxy);
    if (ok) {
//         移除要断开的proxy，无论发生在心跳线程中的proxy副本拷贝前或后，心跳都不会检测到连接错误，不会有proxy断开的通知
//         所以只能在此处通知，不能依靠心跳线程，这里断开通知仅和移除有关
        std::string message("Server " + endpoint + ": " +
            "Disconnected | Connection Id: " +
            boost::lexical_cast<std::string>(connectionId));
        g_connectionCallback(connectionId, -3, message.c_str());

//         使server端到client的心跳失败，在server端回调通知
        proxy->ice_getConnection()->close(
            Ice::ConnectionClose::GracefullyWithWait);

        return 1;
    }
    else {
        return -1;
    }
}

void UvssClient::sendCheckInfo(const std::string& uvssImagePath,
                               const std::string& plateImagePath,
                               const std::string& channel,
                               const std::string& plateNumber,
                               const std::string& direction,
                               const std::string& dateTime,
                               const std::string& extension)
{
    std::vector<std::string> strings;
    std::vector<std::string> fileNames;
    std::vector<std::vector<unsigned char>> files;

    transformCheckInfo(uvssImagePath, plateImagePath, channel, plateNumber, direction, dateTime, extension, strings, fileNames, files);
    proxies_->sendCheckInfo(strings, fileNames, files);
}

void UvssClient::sendCheckInfo(const std::string& endpoint,
                               const std::string& uvssImagePath,
                               const std::string& plateImagePath,
                               const std::string& channel,
                               const std::string& plateNumber,
                               const std::string& direction,
                               const std::string& dateTime,
                               const std::string& extension)
{
    std::vector<std::string> strings;
    std::vector<std::string> fileNames;
    std::vector<std::vector<unsigned char>> files;

    transformCheckInfo(uvssImagePath, plateImagePath, channel, plateNumber, direction, dateTime, extension, strings, fileNames, files);
    proxies_->sendCheckInfo(endpoint, strings, fileNames, files);
}

void UvssClient::sendCheckInfo(const std::string& concatedString,
                               const std::string& concatedFilePath)
{
    std::vector<std::string> strings;
    std::vector<std::string> fileNames;
    std::vector<std::vector<unsigned char>> files;

    transformCheckInfo(concatedString, concatedFilePath, strings, fileNames, files);
    proxies_->sendCheckInfo(strings, fileNames, files);
}

void UvssClient::sendCheckInfo(const std::string& endpoint,
                               const std::string& concatedString,
                               const std::string& concatedFilePath)
{
    std::vector<std::string> strings;
    std::vector<std::string> fileNames;
    std::vector<std::vector<unsigned char>> files;

    transformCheckInfo(concatedString, concatedFilePath, strings, fileNames, files);
    proxies_->sendCheckInfo(endpoint, strings, fileNames, files);
}

void UvssClient::shutdown()
{
    proxies_->destroyHeartbeat();
    queue_->destroy();
    ic_->shutdown();
}

UvssClient::~UvssClient()
{
    proxies_->joinHeartbeat();
    queue_->join();
    ic_->destroy();
}

void UvssClient::filePathToFile(const std::string& filePath,
                                std::vector<unsigned char>& file)
{
    std::ifstream ifs(filePath.c_str(), std::ios::binary);

    ifs.seekg(0, std::ios::end);
    std::streampos fileSize = ifs.tellg();
    file.resize(fileSize);

    ifs.seekg(0, std::ios::beg);
    ifs.read((char*)&file[0], fileSize);
}

void UvssClient::filePathsToFileNamesAndFiles(
    const std::vector<std::string>& filePaths,
    std::vector<std::string>& fileNames,
    std::vector<std::vector<unsigned char>>& files,
    bool isEx)
{
    std::string time = boost::posix_time::to_iso_string(
        boost::posix_time::microsec_clock::local_time());

    if (isEx) {
        for (int i = 0; i != filePaths.size(); ++i) {
            std::string fileName;
            std::vector<unsigned char> file;
            boost::filesystem::path filePath(filePaths[i]);
            if (boost::filesystem::exists(filePath)) {
                fileName = "[" + boost::lexical_cast<std::string>(i + 1) + "]" +
                           filePath.stem().string() + "_" + time +
                           filePath.extension().string();
                filePathToFile(filePaths[i], file);
            }

            fileNames.push_back(fileName);
            files.push_back(file);
        }
    }
    else {
        std::string uvssImagePath(filePaths[0]);
        std::string uvssImageName;
        std::vector<unsigned char> uvssImage;
        boost::filesystem::path filePath(uvssImagePath);
        if (boost::filesystem::exists(filePath)) {
            uvssImageName = "UVSS_" + time + filePath.extension().string();
            filePathToFile(uvssImagePath, uvssImage);
        }

        std::string plateImagePath(filePaths[1]);
        std::string plateImageName;
        std::vector<unsigned char> plateImage;
        boost::filesystem::path filePath1(plateImagePath);
        if (boost::filesystem::exists(filePath1)) {
            plateImageName = "ANPR_" + time + filePath1.extension().string();
            filePathToFile(plateImagePath, plateImage);
        }

        fileNames.push_back(uvssImageName);
        fileNames.push_back(plateImageName);
        files.push_back(uvssImage);
        files.push_back(plateImage);
    }
}

void UvssClient::transformCheckInfo(const std::string& uvssImagePath, const std::string& plateImagePath,
                          const std::string& channel, const std::string& plateNumber,
                          const std::string& direction, const std::string& dateTime,
                          const std::string& extension,
                          std::vector<std::string>& strings,
                          std::vector<std::string>& fileNames,
                          std::vector<std::vector<unsigned char>>& files)
{
    strings.push_back(channel);
    strings.push_back(plateNumber);
    strings.push_back(direction);
    strings.push_back(dateTime);
    strings.push_back(extension);

    std::vector<std::string> filePaths;
    filePaths.push_back(uvssImagePath);
    filePaths.push_back(plateImagePath);

    filePathsToFileNamesAndFiles(filePaths, fileNames, files, false);
}

void UvssClient::transformCheckInfo(const std::string& concatedString, const std::string& concatedFilePath,
                          std::vector<std::string>& strings,
                          std::vector<std::string>& fileNames,
                          std::vector<std::vector<unsigned char>>& files)
{
    strings.push_back(concatedString);

    std::vector<std::string> filePaths;
    const char* concatedFilePathC = concatedFilePath.c_str();
    boost::split(filePaths, concatedFilePathC, boost::is_any_of("|"),
                 boost::token_compress_on);

    filePathsToFileNamesAndFiles(filePaths, fileNames, files, true);
}

#else

UvssClient::UvssClient() :
    queue_(new WorkQueue),
    proxies_(new RpcProxies),
    servant_(new CallbackReceiverI(queue_, proxies_))
{
//     try...catch?
    Ice::PropertiesPtr props = Ice::createProperties();
    props->setProperty("Ice.Warn.Connections", "1");
    props->setProperty("Ice.MessageSizeMax", "0");
//     props->setProperty("Ice.ACM.Client", "0");
    Ice::InitializationData initData;
    initData.properties = props;

    ic_ = Ice::initialize(initData);
    adapter_ = ic_->createObjectAdapter("");
    ident_.name = Ice::generateUUID();
    ident_.category = "";
}

int UvssClient::start()
{
    try {
        adapter_->add(servant_, ident_);
        adapter_->activate();
        queue_->start();
        proxies_->startHeartbeat();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        g_initializationCallback(-1, -1, "Initialization Failed");

        return -1;
    }

    return 1;
}

int UvssClient::connect(const std::string& ipAddress, int port)
{
    try {
        std::string endpoint(ipAddress + ":" +
            boost::lexical_cast<std::string>(port));
        if (proxies_->has(endpoint)) {
            return -2;
        }

        Ice::ObjectPrx base = ic_->stringToProxy("UvssServer:tcp -h " +
            ipAddress + " -p " + boost::lexical_cast<std::string>(port));
        Uvss::CallbackSenderPrx proxy = Ice::checkedCast<Uvss::CallbackSenderPrx>(base);
        if (!proxy) {
            throw std::runtime_error("Invalid proxy");
        }

//         std::cout << proxy->ice_getConnection()->getEndpoint()->toString() << std::endl;
//         std::cout << endpoint << std::endl;
//         tcp -h 192.168.1.9 -p 20145 -t 60000
//         192.168.1.9:20145

//         保留检查对端版本的功能
        if (!proxy->checkVersion(Uvss::version)) {
            return -3;
        }

        proxy->ice_getConnection()->setAdapter(adapter_);
        proxy->addProxy(ident_);

        int connectionId = proxies_->add(proxy, endpoint);

        std::string message("Server " + endpoint + ": " +
            "Connected | Connection Id: " +
            boost::lexical_cast<std::string>(connectionId));
        g_connectionCallback(connectionId, 1, message.c_str());

        return connectionId;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        g_connectionCallback(-1, -2, "Connection Failed");

        return -1;
    }
}

int UvssClient::disconnect(int connectionId)
{
    std::string endpoint;
    Uvss::CallbackSenderPrx proxy;
    bool ok = proxies_->remove(connectionId, endpoint, proxy);
    if (ok) {
//         移除要断开的proxy，无论发生在心跳线程中的proxy副本拷贝前或后，心跳都不会检测到连接错误，不会有proxy断开的通知
//         所以只能在此处通知，不能依靠心跳线程，这里断开通知仅和移除有关
        std::string message("Server " + endpoint + ": " +
            "Disconnected | Connection Id: " +
            boost::lexical_cast<std::string>(connectionId));
        g_connectionCallback(connectionId, -3, message.c_str());

//         使server端到client的心跳失败，在server端回调通知
        proxy->ice_getConnection()->close(
            Ice::ConnectionCloseGracefullyWithWait);

        return 1;
    }
    else {
        return -1;
    }
}

void UvssClient::sendCheckInfo(const std::string& uvssImagePath,
                               const std::string& plateImagePath,
                               const std::string& channel,
                               const std::string& plateNumber,
                               const std::string& direction,
                               const std::string& dateTime,
                               const std::string& extension)
{
    std::vector<std::string> strings;
    std::vector<std::string> fileNames;
    std::vector<std::vector<unsigned char> > files;

    transformCheckInfo(uvssImagePath, plateImagePath, channel, plateNumber, direction, dateTime, extension, strings, fileNames, files);
    proxies_->sendCheckInfo(strings, fileNames, files);
}

void UvssClient::sendCheckInfo(const std::string& endpoint,
                               const std::string& uvssImagePath,
                               const std::string& plateImagePath,
                               const std::string& channel,
                               const std::string& plateNumber,
                               const std::string& direction,
                               const std::string& dateTime,
                               const std::string& extension)
{
    std::vector<std::string> strings;
    std::vector<std::string> fileNames;
    std::vector<std::vector<unsigned char> > files;

    transformCheckInfo(uvssImagePath, plateImagePath, channel, plateNumber, direction, dateTime, extension, strings, fileNames, files);
    proxies_->sendCheckInfo(endpoint, strings, fileNames, files);
}

void UvssClient::sendCheckInfo(const std::string& concatedString,
                               const std::string& concatedFilePath)
{
    std::vector<std::string> strings;
    std::vector<std::string> fileNames;
    std::vector<std::vector<unsigned char> > files;

    transformCheckInfo(concatedString, concatedFilePath, strings, fileNames, files);
    proxies_->sendCheckInfo(strings, fileNames, files);
}

void UvssClient::sendCheckInfo(const std::string& endpoint,
                               const std::string& concatedString,
                               const std::string& concatedFilePath)
{
    std::vector<std::string> strings;
    std::vector<std::string> fileNames;
    std::vector<std::vector<unsigned char> > files;

    transformCheckInfo(concatedString, concatedFilePath, strings, fileNames, files);
    proxies_->sendCheckInfo(endpoint, strings, fileNames, files);
}

void UvssClient::shutdown()
{
    proxies_->destroyHeartbeat();
    queue_->destroy();
    ic_->shutdown();
}

UvssClient::~UvssClient()
{
    proxies_->joinHeartbeat();
    queue_->join();
    ic_->destroy();
}

void UvssClient::filePathToFile(const std::string& filePath,
                                std::vector<unsigned char>& file)
{
    std::ifstream ifs(filePath.c_str(), std::ios::binary);

    ifs.seekg(0, std::ios::end);
    std::streampos fileSize = ifs.tellg();
    file.resize(fileSize);

    ifs.seekg(0, std::ios::beg);
    ifs.read((char*)&file[0], fileSize);
}

void UvssClient::filePathsToFileNamesAndFiles(
    const std::vector<std::string>& filePaths,
    std::vector<std::string>& fileNames,
    std::vector<std::vector<unsigned char> >& files,
    bool isEx)
{
    std::string time = boost::posix_time::to_iso_string(
        boost::posix_time::microsec_clock::local_time());

    if (isEx) {
        for (int i = 0; i != filePaths.size(); ++i) {
            std::string fileName;
            std::vector<unsigned char> file;
            boost::filesystem::path filePath(filePaths[i]);
            if (boost::filesystem::exists(filePath)) {
                fileName = "[" + boost::lexical_cast<std::string>(i + 1) + "]" +
                           filePath.stem().string() + "_" + time +
                           filePath.extension().string();
                filePathToFile(filePaths[i], file);
            }

            fileNames.push_back(fileName);
            files.push_back(file);
        }
    }
    else {
        std::string uvssImagePath(filePaths[0]);
        std::string uvssImageName;
        std::vector<unsigned char> uvssImage;
        boost::filesystem::path filePath(uvssImagePath);
        if (boost::filesystem::exists(filePath)) {
            uvssImageName = "UVSS_" + time + filePath.extension().string();
            filePathToFile(uvssImagePath, uvssImage);
        }

        std::string plateImagePath(filePaths[1]);
        std::string plateImageName;
        std::vector<unsigned char> plateImage;
        boost::filesystem::path filePath1(plateImagePath);
        if (boost::filesystem::exists(filePath1)) {
            plateImageName = "ANPR_" + time + filePath1.extension().string();
            filePathToFile(plateImagePath, plateImage);
        }

        fileNames.push_back(uvssImageName);
        fileNames.push_back(plateImageName);
        files.push_back(uvssImage);
        files.push_back(plateImage);
    }
}

void UvssClient::transformCheckInfo(const std::string& uvssImagePath, const std::string& plateImagePath,
                          const std::string& channel, const std::string& plateNumber,
                          const std::string& direction, const std::string& dateTime,
                          const std::string& extension,
                          std::vector<std::string>& strings,
                          std::vector<std::string>& fileNames,
                          std::vector<std::vector<unsigned char> >& files)
{
    strings.push_back(channel);
    strings.push_back(plateNumber);
    strings.push_back(direction);
    strings.push_back(dateTime);
    strings.push_back(extension);

    std::vector<std::string> filePaths;
    filePaths.push_back(uvssImagePath);
    filePaths.push_back(plateImagePath);

    filePathsToFileNamesAndFiles(filePaths, fileNames, files, false);
}

void UvssClient::transformCheckInfo(const std::string& concatedString, const std::string& concatedFilePath,
                          std::vector<std::string>& strings,
                          std::vector<std::string>& fileNames,
                          std::vector<std::vector<unsigned char> >& files)
{
    strings.push_back(concatedString);

    std::vector<std::string> filePaths;
    const char* concatedFilePathC = concatedFilePath.c_str();
    boost::split(filePaths, concatedFilePathC, boost::is_any_of("|"),
                 boost::token_compress_on);

    filePathsToFileNamesAndFiles(filePaths, fileNames, files, true);
}

#endif
