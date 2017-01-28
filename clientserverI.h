#ifndef __clientserverI_h__
#define __clientserverI_h__

#include <clientserver.h>
//base

namespace UVSS
{

class ClientI : public virtual Client
{
public:

    virtual void writeCheckInfoAsync(::std::string,
                                     ::UVSS::ByteSeq,
                                     ::std::string,
                                     ::UVSS::ByteSeq,
                                     ::std::string,
                                     ::std::string,
                                     ::std::string,
                                     ::std::string,
                                     ::std::string,
                                     std::function<void()>,
                                     std::function<void(std::exception_ptr)>,
                                     const Ice::Current&) override;
};

class ServerI : public virtual Server
{
public:

    virtual void addClient(::Ice::Identity,
                           const Ice::Current&) override;

    virtual bool checkVersion(::std::string,
                              const Ice::Current&) override;
};

}

#endif
