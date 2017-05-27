// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************
#include <boost/filesystem.hpp>
#include <Ice/Ice.h>
#include <WorkQueue.h>

#include <clientserver.h>
#include <fstream>


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
            //可能在两处被唤醒
            //被唤醒后，进入下一轮循环，如果是被destroy唤醒的，if条件不成立，跳出循环
            //如果不是，_callbacks非空，进入else
            _condition.wait(lock);
        }
        else {
            CallbackEntry entry = _callbacks.front();//1
            
//             _condition.wait_for(lock, std::chrono::seconds(5));
            
            _callbacks.pop_front();//2

            std::string uVSSImageName = get<0>(entry);
            UVSS::ByteSeq uVSSImage = get<1>(entry);
            std::string plateImageName = get<2>(entry);
            UVSS::ByteSeq plateImage = get<3>(entry);
//             std::string channel = get<4>(entry);
//             std::string plateNumber = get<5>(entry);
//             std::string direction = get<6>(entry);
//             std::string time = get<7>(entry);
//             std::string extension = get<8>(entry);
            UVSS::StringSeq ss = get<4>(entry);
            int index = get<7>(entry);

            createImageDirectory("UVSS");

            boost::filesystem::path currentPath =
                boost::filesystem::current_path();
            std::string imagePath = currentPath.string() + "/UVSS/";

            std::string uVSSImagePath;
            if (!uVSSImageName.empty()) {
                uVSSImagePath = imagePath + uVSSImageName;
                std::ofstream ofs(uVSSImagePath, std::ios::binary);
                ofs.write((char*)&uVSSImage[0], uVSSImage.size());
            }

            std::string plateImagePath;
            if (!plateImageName.empty()) {
                plateImagePath = imagePath + plateImageName;
                std::ofstream ofs(plateImagePath, std::ios::binary);
                ofs.write((char*)&plateImage[0], plateImage.size());
            }

            int sz = ss.size();
            char** dst = new char*[sz];
            for (int i = 0; i != sz; ++i) {
                int szi = ss[i].size();
                dst[i] = new char[szi + 1];
                strcpy(dst[i], ss[i].c_str());
            }
    
    
            
            this->checkInfoCallback(index,
                                    uVSSImagePath.c_str(), plateImagePath.c_str(),
                                    dst, sz);

            for (int i = 0; i != sz; ++i) {
                delete[] dst[i];
                dst[i] = 0;
            }
            delete[] dst;
            dst = 0;
            
            
            auto& response = get<5>(entry);//4
            
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
            auto& error = get<6>(entry);
            error(current_exception());
        }
    }
}

void
WorkQueue::add(std::string uVSSImageName, UVSS::ByteSeq uVSSImage,
        std::string plateImageName, UVSS::ByteSeq plateImage,
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
        _callbacks.push_back(make_tuple(uVSSImageName, uVSSImage,
                                        plateImageName, plateImage,
                                        ss,
                                        std::move(response),
                                        std::move(error),
                                        index)
                            );
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

