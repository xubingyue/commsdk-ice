#include <workqueue.h>

#include <boost/filesystem.hpp>

#include <global.h>

WorkQueue::WorkQueue() : done_(false)
{
}

void WorkQueue::run()
{
    std::unique_lock<std::mutex> lock(mutex_);

    while (!done_) {
        if (callbacks_.empty()) {
//             可能被add或destroy处唤醒
//             被唤醒后，进入下一轮while循环，
//             如果是被destroy唤醒的，done_== true，while条件不成立，跳出循环
//             如果是被add唤醒的，callbacks_.empty() == false，if条件不成立，进入else分支
            condition_.wait(lock);
        }
        else {
            CallbackEntry& entry = callbacks_.front(); // &?

            createFileDirectory("UVSS");
            boost::filesystem::path currentPath = boost::filesystem::current_path();
            std::string imagePath = currentPath.string() + "/UVSS/";

            auto& filePaths = std::get<1>(entry);
            auto& files = std::get<2>(entry);
            int filePathsSize = filePaths.size();

            for (int i = 0; i != filePathsSize; ++i) {
                if (!filePaths[i].empty()) {
                    filePaths[i] = imagePath + filePaths[i];
                    std::ofstream ofs(filePaths[i], std::ios::binary);
                    ofs.write((char*)&files[i][0], files[i].size());
                }
            }

            char** filePathsC = new char* [filePathsSize + 1];
            for (int i = 0; i != filePathsSize; ++i) {
                int szi = filePaths[i].size();
                filePathsC[i] = new char[szi + 1];
                strcpy(filePathsC[i], filePaths[i].c_str());
            }
            filePathsC[filePathsSize] = NULL;

            auto& strings = std::get<0>(entry);
            int stringsSize = strings.size();

            char** stringsC = new char*[stringsSize + 1];
            for (int i = 0; i != stringsSize; ++i) {
                int szi = strings[i].size();
                stringsC[i] = new char[szi + 1];
                strcpy(stringsC[i], strings[i].c_str());
            }
            stringsC[stringsSize] = NULL;

            int& connectionId = std::get<3>(entry);

            if (g_type == 0) {
                g_checkInfoCallback(connectionId,
                    filePathsC[0], filePathsC[1],
                    stringsC[0], stringsC[1], stringsC[2], stringsC[3], stringsC[4]
                );
            }
            else if (g_type == 1) {
                g_checkInfoCallbackNormal(connectionId, filePathsC, stringsC);
            }
            else {
                std::string filePathsDst;
                for (std::vector<std::string>::const_iterator it = filePaths.begin();
                        it != filePaths.end() - 1; ++it) {
                    filePathsDst += *it + "#";
                }
                filePathsDst += filePaths[filePaths.size() - 1];
                g_checkInfoCallbackNew(connectionId, filePathsDst.c_str(), stringsC[0]);
            }

            auto& response = std::get<4>(entry);
            response();
            callbacks_.pop_front();

            for (int i = 0; i != filePathsSize + 1; ++i) {
                delete[] filePathsC[i];
                filePathsC[i] = 0;
            }
            delete[] filePathsC;
            filePathsC = 0;

            for (int i = 0; i != stringsSize + 1; ++i) {
                delete[] stringsC[i];
                stringsC[i] = 0;
            }
            delete[] stringsC;
            stringsC = 0;
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
        run();
    });
    thread_ = std::move(t);
}

void WorkQueue::add(
    const std::vector<std::string>& strings,
    const std::vector<std::string>& fileNames,
    const std::vector<std::vector<unsigned char>>& files,
    int index,
    std::function<void ()> response,
    std::function<void (std::exception_ptr)> error)
{
    std::unique_lock<std::mutex> lock(mutex_);

//     destroy后仍然有可能执行add
//     所以要判断if _done
    if (!done_) {
        if (callbacks_.size() == 0) {
            condition_.notify_one();
        }
        callbacks_.push_back(make_tuple(std::move(strings),
                                        std::move(fileNames),
                                        std::move(files),
                                        index,
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

void WorkQueue::createFileDirectory(const std::string& fileDirectory)
{
    boost::filesystem::path dir(fileDirectory);

    if (boost::filesystem::exists(dir)) {
        return;
    }
    else {
        boost::filesystem::create_directory(dir);
    }
}

void WorkQueue::destroy()
{
    std::unique_lock<std::mutex> lock(mutex_);

    done_ = true;
    condition_.notify_one();
}

void WorkQueue::join()
{
    if (thread_.joinable()) {
        thread_.join();
    }
}
