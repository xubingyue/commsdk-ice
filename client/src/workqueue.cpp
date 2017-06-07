#include <workqueue.h>

#include <boost/filesystem.hpp>

CheckInfoCallback WorkQueue::checkInfoCallback_ = 0;

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
            CallbackEntry entry = callbacks_.front();




            callbacks_.pop_front();

            auto& fileNames = std::get<0>(entry);
            auto& files = std::get<1>(entry);
            auto& strings = std::get<2>(entry);
            int& index = std::get<5>(entry);

            createImageDirectory("UVSS");

            boost::filesystem::path currentPath =
                boost::filesystem::current_path();
            std::string imagePath = currentPath.string() + "/UVSS/";

            int nsz = fileNames.size();
            for (int i = 0; i != nsz; ++i) {
                if (!fileNames[i].empty()) {
                    fileNames[i] = imagePath + fileNames[i];
                    std::ofstream ofs(fileNames[i], std::ios::binary);
                    ofs.write((char*)&files[i][0], files[i].size());
                }
            }

            char** dst1 = new char*[nsz];
            for (int i = 0; i != nsz; ++i) {
                int szi = fileNames[i].size();
                dst1[i] = new char[szi + 1];
                strcpy(dst1[i], fileNames[i].c_str());
            }


            int sz = strings.size();
            char** dst = new char*[sz];
            for (int i = 0; i != sz; ++i) {
                int szi = strings[i].size();
                dst[i] = new char[szi + 1];
                strcpy(dst[i], strings[i].c_str());
            }

            this->checkInfoCallback_(index,
                                     dst1, nsz,
                                     dst, sz);

            for (int i = 0; i != sz; ++i) {
                delete[] dst[i];
                dst[i] = 0;
            }
            delete[] dst;
            dst = 0;

            auto& response = std::get<3>(entry);//4

            response();//5
        }
    }

    //
    // Throw exception for any outstanding requests.
    //
    for(auto& entry : callbacks_)
    {
        try
        {
            throw Uvss::RequestCanceledException();
        }
        catch(...)
        {
            std::cerr << "yyyyyyyyyyyyyyyyyyyyyyyy" << std::endl;
            auto& error = std::get<4>(entry);
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
    thread_ = std::move(t);
}

void WorkQueue::add(
    const std::vector<std::string>& ns,
    const std::vector<std::vector<unsigned char>>& bss,
    const std::vector<std::string>& ss,
    std::function<void ()> response, std::function<void (std::exception_ptr)> error,
    int index)
{
    //destroy后仍然有可能执行add
    //所以要判断if _done

    std::unique_lock<std::mutex> lock(mutex_);

    if(!done_)
    {
        //
        // Add work item.
        //
        if(callbacks_.size() == 0)//rugo == 1 shi huanxing run.wait
        {
            condition_.notify_one();
        }
        callbacks_.push_back(make_tuple(ns,
                                        bss,
                                        ss,
                                        std::move(response),
                                        std::move(error),
                                        index));
    }
    else
    {
        //
        // Destroyed, throw exception.
        //
        try
        {
            throw Uvss::RequestCanceledException();
        }
        catch(...)
        {
            std::cerr << "nnnnnnnnnnnnnnnnnnnnn" << std::endl;
            error(std::current_exception());
        }
    }
}

void WorkQueue::createImageDirectory(const std::string& imageDirectory)
{
    boost::filesystem::path dir(imageDirectory);

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

    //
    // Set done flag and notify.
    //
    done_ = true;
    condition_.notify_one();
}

void WorkQueue::join()
{
    if(thread_.joinable())
    {
        thread_.join();
    }
}

void WorkQueue::setCheckInfoCallback(CheckInfoCallback checkInfoCallback)
{
    WorkQueue::checkInfoCallback_ = checkInfoCallback;
}
