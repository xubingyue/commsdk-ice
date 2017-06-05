#include <workqueue.h>

#include <boost/filesystem.hpp>

using namespace std;

UVSSCheckInfoCallback WorkQueue::checkInfoCallback = 0;

WorkQueue::WorkQueue() : _done(false)
{
}

void
WorkQueue::start()
{
    thread t([this]()
    {
        this->run();
    });
    _thread = move(t);
}

void
WorkQueue::join()
{
    if(_thread.joinable())
    {
        _thread.join();
    }
}

void
WorkQueue::run()
{
    unique_lock<mutex> lock(_mutex);

    while(!_done)
    {
        if(_callbacks.empty()) {
//             可能在两处被唤醒
//             被唤醒后，进入下一轮循环，如果是被destroy唤醒的，if条件不成立，跳出循环
//             如果不是，_callbacks非空，进入else
            _condition.wait(lock);
        }
        else {
            CallbackEntry entry = _callbacks.front();//1

//             _condition.wait_for(lock, std::chrono::seconds(5));

            _callbacks.pop_front();//2

            UVSS::StringSeq ns = get<0>(entry);
            UVSS::ByteSeqSeq bss = get<1>(entry);
            UVSS::StringSeq ss = get<2>(entry);
            int index = get<5>(entry);

            createImageDirectory("UVSS");

            boost::filesystem::path currentPath =
                boost::filesystem::current_path();
            std::string imagePath = currentPath.string() + "/UVSS/";

            int nsz = ns.size();
            for (int i = 0; i != nsz; ++i) {
                if (!ns[i].empty()) {
                    ns[i] = imagePath + ns[i];
                    std::ofstream ofs(ns[i], std::ios::binary);
                    ofs.write((char*)&bss[i][0], bss[i].size());
                }
            }

            char** dst1 = new char*[nsz];
            for (int i = 0; i != nsz; ++i) {
                int szi = ns[i].size();
                dst1[i] = new char[szi + 1];
                strcpy(dst1[i], ns[i].c_str());
            }


            int sz = ss.size();
            char** dst = new char*[sz];
            for (int i = 0; i != sz; ++i) {
                int szi = ss[i].size();
                dst[i] = new char[szi + 1];
                strcpy(dst[i], ss[i].c_str());
            }

            this->checkInfoCallback(index,
                                    dst1, nsz,
                                    dst, sz);

            for (int i = 0; i != sz; ++i) {
                delete[] dst[i];
                dst[i] = 0;
            }
            delete[] dst;
            dst = 0;

            auto& response = get<3>(entry);//4

            response();//5
        }
    }

    //
    // Throw exception for any outstanding requests.
    //
    for(auto& entry : _callbacks)
    {
        try
        {
            throw UVSS::RequestCanceledException();
        }
        catch(...)
        {
            std::cerr << "yyyyyyyyyyyyyyyyyyyyyyyy" << std::endl;
            auto& error = get<4>(entry);
            error(current_exception());
        }
    }
}

void
WorkQueue::add(
    UVSS::StringSeq ns,
    UVSS::ByteSeqSeq bss,
    UVSS::StringSeq ss,
    std::function<void ()> response, std::function<void (exception_ptr)> error,
    int index)
{
    //destroy后仍然有可能执行add
    //所以要判断if _done

    unique_lock<mutex> lock(_mutex);

    if(!_done)
    {
        //
        // Add work item.
        //
        if(_callbacks.size() == 0)//rugo == 1 shi huanxing run.wait
        {
            _condition.notify_one();
        }
        _callbacks.push_back(make_tuple(ns,
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
            throw UVSS::RequestCanceledException();
        }
        catch(...)
        {
            std::cerr << "nnnnnnnnnnnnnnnnnnnnn" << std::endl;
            error(current_exception());
        }
    }
}

void
WorkQueue::destroy()
{
    unique_lock<mutex> lock(_mutex);

    //
    // Set done flag and notify.
    //
    _done = true;
    _condition.notify_one();
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

void WorkQueue::setCheckInfoCallback(UVSSCheckInfoCallback checkInfoCallback)
{
    WorkQueue::checkInfoCallback = checkInfoCallback;
}

