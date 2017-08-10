#include <workqueue.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include <global.h>

WorkQueue::WorkQueue() : destroy_(false)
{
}

#ifdef ICE_CPP11_MAPPING

void WorkQueue::run()
{
    std::unique_lock<std::mutex> lock(mutex_);

    while (!destroy_) {
        if (callbacks_.empty()) {
//             可能被add或destroy处唤醒
//             被唤醒后，进入下一轮while循环
//             如果是被destroy唤醒的，destroy_ == true，while条件不成立，跳出循环
//             如果是被add唤醒的，callbacks_.empty() == false，if条件不成立，进入else分支
            condition_.wait(lock);
        }
        else {
            CallbackEntry& entry = callbacks_.front();

            auto& connectionId = std::get<0>(entry);
            auto& strings = std::get<1>(entry);
            auto& fileNames = std::get<2>(entry);
            auto& files = std::get<3>(entry);

            auto& filePaths = fileNames;
            fileNamesAndFilesTofilePaths(fileNames, files, "UVSS", connectionId,
                                         filePaths);

            if (strings.size() == 5 && filePaths.size() == 2) {
                g_checkInfoCallback(
                    connectionId,
                    filePaths[0].c_str(), filePaths[1].c_str(),
                    strings[0].c_str(), strings[1].c_str(), strings[2].c_str(),
                    strings[3].c_str(), strings[4].c_str());
            }
            else if (strings.size() == 1){
                std::string concatedFilePath =
                    boost::algorithm::join(filePaths, "|");
                g_checkInfoExCallback(
                    connectionId,
                    strings[0].c_str(),
                    concatedFilePath.c_str());
            }
            else {
            }

            auto& response = std::get<4>(entry);
            response();
            callbacks_.pop_front();
        }
    }

    for (auto& entry : callbacks_) {
        try {
            throw Uvss::RequestCanceledException();
        }
        catch (...) {
            auto& error = std::get<5>(entry);
            error(std::current_exception());
        }
    }
}

void WorkQueue::start()
{
    std::thread t([this]()
    {
        this->run();
    });
    workthread_ = std::move(t);
}

void WorkQueue::add(
    int index,
    const std::vector<std::string>& strings,
    const std::vector<std::string>& fileNames,
    const std::vector<std::vector<unsigned char>>& files,
    std::function<void ()> response,
    std::function<void (std::exception_ptr)> error)
{
    std::unique_lock<std::mutex> lock(mutex_);

    if (!destroy_) { // destroy后仍然有可能执行add 所以要判断if destroy_
        if (callbacks_.size() == 0) {
            condition_.notify_one();
        }
        callbacks_.push_back(make_tuple(index,std::move(strings),
                                        std::move(fileNames),
                                        std::move(files),
                                        std::move(response),
                                        std::move(error)));
    }
    else {
        try {
            throw Uvss::RequestCanceledException();
        }
        catch (...) {
            error(std::current_exception());
        }
    }
}

void WorkQueue::destroy()
{
    std::unique_lock<std::mutex> lock(mutex_);
    destroy_ = true;
    condition_.notify_one();
}

void WorkQueue::join()
{
    if (workthread_.joinable()) {
        workthread_.join();
    }
}

std::string WorkQueue::fileDirectory(const std::string& folder)
{
    boost::filesystem::path dir(folder);
    if (!boost::filesystem::exists(dir)) {
        boost::filesystem::create_directory(dir);
    }
    boost::filesystem::path currentPath = boost::filesystem::current_path();
    std::string fileDir = currentPath.string() + SLASH + folder;

    return fileDir;
}

void WorkQueue::fileNamesAndFilesTofilePaths(
    std::vector<std::string>& fileNames,
    const std::vector<std::vector<unsigned char>>& files,
    const std::string& folder,
    int connectionId,
    std::vector<std::string>& filePaths)
{
    std::string fileDir = fileDirectory(folder);

    for (int i = 0; i != fileNames.size(); ++i) {
        if (!fileNames[i].empty()) {
            filePaths[i] = fileDir + SLASH + "[" +
                boost::lexical_cast<std::string>(connectionId) + "]" +
                fileNames[i];
            std::ofstream ofs(filePaths[i].c_str(), std::ios::binary);
            ofs.write((char*)&files[i][0], files[i].size());
        }
    }
}

#else

#include <boost/move/utility_core.hpp>

void WorkQueue::run()
{
    boost::unique_lock<boost::mutex> lock(mutex_);

    while (!destroy_) {
        if (callbacks_.empty()) {
//             可能被add或destroy处唤醒
//             被唤醒后，进入下一轮while循环
//             如果是被destroy唤醒的，destroy_ == true，while条件不成立，跳出循环
//             如果是被add唤醒的，callbacks_.empty() == false，if条件不成立，进入else分支
            condition_.wait(lock);
        }
        else {
            CallbackEntry& entry = callbacks_.front();

            int& connectionId = entry.index;
            std::vector<std::string>& strings = entry.strings;
            std::vector<std::string>& fileNames = entry.fileNames;
            std::vector<std::vector<unsigned char> >& files = entry.files;

            std::vector<std::string>& filePaths = fileNames;
            fileNamesAndFilesTofilePaths(fileNames, files, "UVSS", connectionId,
                                         filePaths);

            if (strings.size() == 5 && filePaths.size() == 2) {
                g_checkInfoCallback(
                    connectionId,
                    filePaths[0].c_str(), filePaths[1].c_str(),
                    strings[0].c_str(), strings[1].c_str(), strings[2].c_str(),
                    strings[3].c_str(), strings[4].c_str());
            }
            else if (strings.size() == 1){
                std::string concatedFilePath =
                    boost::algorithm::join(filePaths, "|");
                g_checkInfoExCallback(
                    connectionId,
                    strings[0].c_str(),
                    concatedFilePath.c_str());
            }
            else {
            }

            entry.cb->ice_response();
            callbacks_.pop_front();
        }
    }

    for (std::list<CallbackEntry>::const_iterator
        p = callbacks_.begin(); p != callbacks_.end(); ++p) {
        p->cb->ice_exception(Uvss::RequestCanceledException());
    }
}

void WorkQueue::start()
{
    boost::function0<void> f = boost::bind(&WorkQueue::run, this);
    boost::thread t(f);
    workthread_ = boost::move(t);
}

void WorkQueue::add(
    const Uvss::AMD_CallbackSender_sendDataPtr& cb,
    int index,
    const std::vector<std::string>& strings,
    const std::vector<std::string>& fileNames,
    const std::vector<std::vector<unsigned char> >& files)
{
    boost::unique_lock<boost::mutex> lock(mutex_);

    if (!destroy_) { // destroy后仍然有可能执行add 所以要判断if destroy_
        if (callbacks_.size() == 0) {
            condition_.notify_one();
        }
        CallbackEntry entry;
        entry.cb = boost::move(cb);
        entry.index = boost::move(index);
        entry.strings = boost::move(strings);
        entry.fileNames = boost::move(fileNames);
        entry.files = boost::move(files);
        callbacks_.push_back(entry);
    }
    else {
        cb->ice_exception(Uvss::RequestCanceledException());
    }
}

void WorkQueue::destroy()
{
    boost::unique_lock<boost::mutex> lock(mutex_);
    destroy_ = true;
    condition_.notify_one();
}

void WorkQueue::join()
{
    if (workthread_.joinable()) {
        workthread_.join();
    }
}

std::string WorkQueue::fileDirectory(const std::string& folder)
{
    boost::filesystem::path dir(folder);
    if (!boost::filesystem::exists(dir)) {
        boost::filesystem::create_directory(dir);
    }
    boost::filesystem::path currentPath = boost::filesystem::current_path();
    std::string fileDir = currentPath.string() + SLASH + folder;

    return fileDir;
}

void WorkQueue::fileNamesAndFilesTofilePaths(
    std::vector<std::string>& fileNames,
    const std::vector<std::vector<unsigned char> >& files,
    const std::string& folder,
    int connectionId,
    std::vector<std::string>& filePaths)
{
    std::string fileDir = fileDirectory(folder);

    for (int i = 0; i != fileNames.size(); ++i) {
        if (!fileNames[i].empty()) {
            filePaths[i] = fileDir + SLASH + "[" +
                boost::lexical_cast<std::string>(connectionId) + "]" +
                fileNames[i];
            std::ofstream ofs(filePaths[i].c_str(), std::ios::binary);
            ofs.write((char*)&files[i][0], files[i].size());
        }
    }
}

#endif
