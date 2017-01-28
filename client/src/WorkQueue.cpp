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

ClientCheckInfoCallback WorkQueue::checkInfoCallback = 0;

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
        if(_callbacks.empty())
        {
            _condition.wait(lock);//maybe awake by 2 places, bei destroy huanxinghou kenenghaishikongde!
        }
        else
//         if(!_callbacks.empty())//NB
        {
            //
            // Get next work item.
            //
            CallbackEntry entry = _callbacks.front();//1

            //
            // Wait for the amount of time indicated in delay to
            // emulate a process that takes a significant period of
            // time to complete.
            //
//             _condition.wait_for(lock, chrono::milliseconds(get<0>(entry)));//2 places awake//uvss qudiao!

//             if(!_done)//uvss zheli keyi qudiao!!!
//             {
            //
            // Print greeting and send response.
            //
            _callbacks.pop_front();//2
//             cout << "Belated Hello World!" << endl;//3

            std::string uVSSImageName = get<0>(entry);
            UVSS::ByteSeq uVSSImage = get<1>(entry);
            std::string plateImageName = get<2>(entry);
            UVSS::ByteSeq plateImage = get<3>(entry);
            std::string channel = get<4>(entry);
            std::string plateNumber = get<5>(entry);
            std::string direction = get<6>(entry);
            std::string time = get<7>(entry);
            std::string extension = get<8>(entry);
            int index = get<11>(entry);

//             std::cout << uVSSImageName << std::endl;
//             std::cout << time << std::endl;
//             std::cout << index << std::endl;

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



            this->checkInfoCallback(index,
                                    uVSSImagePath.c_str(), plateImagePath.c_str(),
                                    channel.c_str(), plateNumber.c_str(), direction.c_str(),
                                    time.c_str(), extension.c_str());


            auto& response = get<9>(entry);//4
            response();//5
//             }
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
            auto& error = get<10>(entry);
            error(current_exception());
        }
    }
}

void
WorkQueue::add(std::string uVSSImageName, UVSS::ByteSeq uVSSImage,
        std::string plateImageName, UVSS::ByteSeq plateImage,
        std::string channel, std::string plateNumber,
        std::string direction, std::string time,
        std::string extension,
        std::function<void ()> response, std::function<void (exception_ptr)> error,
        int index)
{
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
                                        channel,
                                        plateNumber,
                                        direction,
                                        time,
                                        extension,
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
    _condition.notify_one();//2 places maybe awake
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

void WorkQueue::setCheckInfoCallback(ClientCheckInfoCallback checkInfoCallback)
{
    WorkQueue::checkInfoCallback = checkInfoCallback;
}

