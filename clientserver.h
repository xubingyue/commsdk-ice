// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.7a4
//
// <auto-generated>
//
// Generated from file `clientserver.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#ifndef __clientserver_h__
#define __clientserver_h__

#include <IceUtil/PushDisableWarnings.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/ValueF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/StreamHelpers.h>
#include <Ice/Comparable.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/GCObject.h>
#include <Ice/Value.h>
#include <Ice/Incoming.h>
#include <Ice/IncomingAsync.h>
#include <Ice/FactoryTableInit.h>
#include <IceUtil/ScopedArray.h>
#include <Ice/Optional.h>
#include <Ice/ExceptionHelpers.h>
#include <Ice/Identity.h>
#include <IceUtil/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION  != 30754
#       error Ice version mismatch: an exact match is required for beta generated code
#   endif
#endif

#ifdef ICE_CPP11_MAPPING // C++11 mapping

namespace UVSS
{

class Client;
class ClientPrx;
class Server;
class ServerPrx;

}

namespace UVSS
{

using ByteSeq = ::std::vector<::Ice::Byte>;

class RequestCanceledException : public ::Ice::UserExceptionHelper<RequestCanceledException, ::Ice::UserException>
{
public:

    virtual ~RequestCanceledException();

    RequestCanceledException(const RequestCanceledException&) = default;

    RequestCanceledException() = default;

    std::tuple<> ice_tuple() const
    {
        return std::tie();
    }

    static const ::std::string& ice_staticId();
};

static RequestCanceledException _iceS_RequestCanceledException_init;

}

namespace UVSS
{

class Client : public virtual ::Ice::Object
{
public:

    using ProxyType = ClientPrx;

    virtual bool ice_isA(::std::string, const ::Ice::Current& = ::Ice::noExplicitCurrent) const override;
    virtual ::std::vector<::std::string> ice_ids(const ::Ice::Current& = ::Ice::noExplicitCurrent) const override;
    virtual ::std::string ice_id(const ::Ice::Current& = ::Ice::noExplicitCurrent) const override;

    static const ::std::string& ice_staticId();

    virtual void writeCheckInfoAsync(::std::string, ::UVSS::ByteSeq, ::std::string, ::UVSS::ByteSeq, ::std::string, ::std::string, ::std::string, ::std::string, ::std::string, ::std::function<void()>, ::std::function<void(::std::exception_ptr)>, const ::Ice::Current& = ::Ice::noExplicitCurrent) = 0;
    bool _iceD_writeCheckInfo(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool _iceDispatch(::IceInternal::Incoming&, const ::Ice::Current&) override;
};

class Server : public virtual ::Ice::Object
{
public:

    using ProxyType = ServerPrx;

    virtual bool ice_isA(::std::string, const ::Ice::Current& = ::Ice::noExplicitCurrent) const override;
    virtual ::std::vector<::std::string> ice_ids(const ::Ice::Current& = ::Ice::noExplicitCurrent) const override;
    virtual ::std::string ice_id(const ::Ice::Current& = ::Ice::noExplicitCurrent) const override;

    static const ::std::string& ice_staticId();

    virtual bool checkVersion(::std::string, const ::Ice::Current& = ::Ice::noExplicitCurrent) = 0;
    bool _iceD_checkVersion(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void addClient(::Ice::Identity, const ::Ice::Current& = ::Ice::noExplicitCurrent) = 0;
    bool _iceD_addClient(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool _iceDispatch(::IceInternal::Incoming&, const ::Ice::Current&) override;
};

}

namespace UVSS
{

class ClientPrx : public virtual ::Ice::Proxy<ClientPrx, ::Ice::ObjectPrx>
{
public:

    void writeCheckInfo(const ::std::string& iceP_uVSSImageName, const ::UVSS::ByteSeq& iceP_uVSSImage, const ::std::string& iceP_plateImageName, const ::UVSS::ByteSeq& iceP_plateImage, const ::std::string& iceP_channel, const ::std::string& iceP_plateNumber, const ::std::string& iceP_direction, const ::std::string& iceP_time, const ::std::string& iceP_extension, const ::Ice::Context& context = Ice::noExplicitContext)
    {
        _makePromiseOutgoing<void>(true, this, &UVSS::ClientPrx::_iceI_writeCheckInfo, iceP_uVSSImageName, iceP_uVSSImage, iceP_plateImageName, iceP_plateImage, iceP_channel, iceP_plateNumber, iceP_direction, iceP_time, iceP_extension, context).get();
    }

    template<template<typename> class P = ::std::promise>
    auto writeCheckInfoAsync(const ::std::string& iceP_uVSSImageName, const ::UVSS::ByteSeq& iceP_uVSSImage, const ::std::string& iceP_plateImageName, const ::UVSS::ByteSeq& iceP_plateImage, const ::std::string& iceP_channel, const ::std::string& iceP_plateNumber, const ::std::string& iceP_direction, const ::std::string& iceP_time, const ::std::string& iceP_extension, const ::Ice::Context& context = Ice::noExplicitContext)
        -> decltype(::std::declval<P<void>>().get_future())
    {
        return _makePromiseOutgoing<void, P>(false, this, &UVSS::ClientPrx::_iceI_writeCheckInfo, iceP_uVSSImageName, iceP_uVSSImage, iceP_plateImageName, iceP_plateImage, iceP_channel, iceP_plateNumber, iceP_direction, iceP_time, iceP_extension, context);
    }

    ::std::function<void()>
    writeCheckInfoAsync(const ::std::string& iceP_uVSSImageName, const ::UVSS::ByteSeq& iceP_uVSSImage, const ::std::string& iceP_plateImageName, const ::UVSS::ByteSeq& iceP_plateImage, const ::std::string& iceP_channel, const ::std::string& iceP_plateNumber, const ::std::string& iceP_direction, const ::std::string& iceP_time, const ::std::string& iceP_extension, 
                        ::std::function<void()> response,
                        ::std::function<void(::std::exception_ptr)> ex = nullptr,
                        ::std::function<void(bool)> sent = nullptr,
                        const ::Ice::Context& context = Ice::noExplicitContext)
    {
        return _makeLamdaOutgoing<void>(response, ex, sent, this, &UVSS::ClientPrx::_iceI_writeCheckInfo, iceP_uVSSImageName, iceP_uVSSImage, iceP_plateImageName, iceP_plateImage, iceP_channel, iceP_plateNumber, iceP_direction, iceP_time, iceP_extension, context);
    }

    void _iceI_writeCheckInfo(const ::std::shared_ptr<::IceInternal::OutgoingAsyncT<void>>&, const ::std::string&, const ::UVSS::ByteSeq&, const ::std::string&, const ::UVSS::ByteSeq&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    static const ::std::string& ice_staticId();

protected: 

    ClientPrx() = default;
    friend ::std::shared_ptr<ClientPrx> IceInternal::createProxy<ClientPrx>();

    virtual ::std::shared_ptr<::Ice::ObjectPrx> _newInstance() const override;
};

class ServerPrx : public virtual ::Ice::Proxy<ServerPrx, ::Ice::ObjectPrx>
{
public:

    bool checkVersion(const ::std::string& iceP_ver, const ::Ice::Context& context = Ice::noExplicitContext)
    {
        return _makePromiseOutgoing<bool>(true, this, &UVSS::ServerPrx::_iceI_checkVersion, iceP_ver, context).get();
    }

    template<template<typename> class P = ::std::promise>
    auto checkVersionAsync(const ::std::string& iceP_ver, const ::Ice::Context& context = Ice::noExplicitContext)
        -> decltype(::std::declval<P<bool>>().get_future())
    {
        return _makePromiseOutgoing<bool, P>(false, this, &UVSS::ServerPrx::_iceI_checkVersion, iceP_ver, context);
    }

    ::std::function<void()>
    checkVersionAsync(const ::std::string& iceP_ver, 
                      ::std::function<void(bool)> response,
                      ::std::function<void(::std::exception_ptr)> ex = nullptr,
                      ::std::function<void(bool)> sent = nullptr,
                      const ::Ice::Context& context = Ice::noExplicitContext)
    {
        return _makeLamdaOutgoing<bool>(response, ex, sent, this, &UVSS::ServerPrx::_iceI_checkVersion, iceP_ver, context);
    }

    void _iceI_checkVersion(const ::std::shared_ptr<::IceInternal::OutgoingAsyncT<bool>>&, const ::std::string&, const ::Ice::Context&);

    void addClient(const ::Ice::Identity& iceP_id, const ::Ice::Context& context = Ice::noExplicitContext)
    {
        _makePromiseOutgoing<void>(true, this, &UVSS::ServerPrx::_iceI_addClient, iceP_id, context).get();
    }

    template<template<typename> class P = ::std::promise>
    auto addClientAsync(const ::Ice::Identity& iceP_id, const ::Ice::Context& context = Ice::noExplicitContext)
        -> decltype(::std::declval<P<void>>().get_future())
    {
        return _makePromiseOutgoing<void, P>(false, this, &UVSS::ServerPrx::_iceI_addClient, iceP_id, context);
    }

    ::std::function<void()>
    addClientAsync(const ::Ice::Identity& iceP_id, 
                   ::std::function<void()> response,
                   ::std::function<void(::std::exception_ptr)> ex = nullptr,
                   ::std::function<void(bool)> sent = nullptr,
                   const ::Ice::Context& context = Ice::noExplicitContext)
    {
        return _makeLamdaOutgoing<void>(response, ex, sent, this, &UVSS::ServerPrx::_iceI_addClient, iceP_id, context);
    }

    void _iceI_addClient(const ::std::shared_ptr<::IceInternal::OutgoingAsyncT<void>>&, const ::Ice::Identity&, const ::Ice::Context&);

    static const ::std::string& ice_staticId();

protected: 

    ServerPrx() = default;
    friend ::std::shared_ptr<ServerPrx> IceInternal::createProxy<ServerPrx>();

    virtual ::std::shared_ptr<::Ice::ObjectPrx> _newInstance() const override;
};

}

namespace Ice
{

}

namespace UVSS
{

using ClientPtr = ::std::shared_ptr<Client>;
using ClientPrxPtr = ::std::shared_ptr<ClientPrx>;

using ServerPtr = ::std::shared_ptr<Server>;
using ServerPrxPtr = ::std::shared_ptr<ServerPrx>;

}

#else // C++98 mapping

namespace IceProxy
{

namespace UVSS
{

class Client;
void _readProxy(::Ice::InputStream*, ::IceInternal::ProxyHandle< ::IceProxy::UVSS::Client>&);
::IceProxy::Ice::Object* upCast(::IceProxy::UVSS::Client*);

class Server;
void _readProxy(::Ice::InputStream*, ::IceInternal::ProxyHandle< ::IceProxy::UVSS::Server>&);
::IceProxy::Ice::Object* upCast(::IceProxy::UVSS::Server*);

}

}

namespace UVSS
{

class Client;
::Ice::Object* upCast(::UVSS::Client*);
typedef ::IceInternal::Handle< ::UVSS::Client> ClientPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::UVSS::Client> ClientPrx;
typedef ClientPrx ClientPrxPtr;
void _icePatchObjectPtr(ClientPtr&, const ::Ice::ObjectPtr&);

class Server;
::Ice::Object* upCast(::UVSS::Server*);
typedef ::IceInternal::Handle< ::UVSS::Server> ServerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::UVSS::Server> ServerPrx;
typedef ServerPrx ServerPrxPtr;
void _icePatchObjectPtr(ServerPtr&, const ::Ice::ObjectPtr&);

}

namespace UVSS
{

typedef ::std::vector< ::Ice::Byte> ByteSeq;

class RequestCanceledException : public ::Ice::UserException
{
public:

    RequestCanceledException() {}
    virtual ~RequestCanceledException() throw();

    virtual ::std::string ice_id() const;
    virtual RequestCanceledException* ice_clone() const;
    virtual void ice_throw() const;

protected:

    virtual void _writeImpl(::Ice::OutputStream*) const;
    virtual void _readImpl(::Ice::InputStream*);
};

static RequestCanceledException _iceS_RequestCanceledException_init;

}

namespace UVSS
{

class AMD_Client_writeCheckInfo : public virtual ::Ice::AMDCallback
{
public:

    virtual ~AMD_Client_writeCheckInfo();

    virtual void ice_response() = 0;
};

typedef ::IceUtil::Handle< ::UVSS::AMD_Client_writeCheckInfo> AMD_Client_writeCheckInfoPtr;

}

namespace IceAsync
{

namespace UVSS
{

class AMD_Client_writeCheckInfo : public ::UVSS::AMD_Client_writeCheckInfo, public ::IceInternal::IncomingAsync
{
public:

    AMD_Client_writeCheckInfo(::IceInternal::Incoming&);

    virtual void ice_response();
};

}

}

namespace UVSS
{

class Callback_Client_writeCheckInfo_Base : public virtual ::IceInternal::CallbackBase { };
typedef ::IceUtil::Handle< Callback_Client_writeCheckInfo_Base> Callback_Client_writeCheckInfoPtr;

class Callback_Server_checkVersion_Base : public virtual ::IceInternal::CallbackBase { };
typedef ::IceUtil::Handle< Callback_Server_checkVersion_Base> Callback_Server_checkVersionPtr;

class Callback_Server_addClient_Base : public virtual ::IceInternal::CallbackBase { };
typedef ::IceUtil::Handle< Callback_Server_addClient_Base> Callback_Server_addClientPtr;

}

namespace IceProxy
{

namespace UVSS
{

class Client : public virtual ::Ice::Proxy<Client, ::IceProxy::Ice::Object>
{
public:

    void writeCheckInfo(const ::std::string& iceP_uVSSImageName, const ::UVSS::ByteSeq& iceP_uVSSImage, const ::std::string& iceP_plateImageName, const ::UVSS::ByteSeq& iceP_plateImage, const ::std::string& iceP_channel, const ::std::string& iceP_plateNumber, const ::std::string& iceP_direction, const ::std::string& iceP_time, const ::std::string& iceP_extension, const ::Ice::Context& context = ::Ice::noExplicitContext)
    {
        end_writeCheckInfo(_iceI_begin_writeCheckInfo(iceP_uVSSImageName, iceP_uVSSImage, iceP_plateImageName, iceP_plateImage, iceP_channel, iceP_plateNumber, iceP_direction, iceP_time, iceP_extension, context, ::IceInternal::dummyCallback, 0, true));
    }

    ::Ice::AsyncResultPtr begin_writeCheckInfo(const ::std::string& iceP_uVSSImageName, const ::UVSS::ByteSeq& iceP_uVSSImage, const ::std::string& iceP_plateImageName, const ::UVSS::ByteSeq& iceP_plateImage, const ::std::string& iceP_channel, const ::std::string& iceP_plateNumber, const ::std::string& iceP_direction, const ::std::string& iceP_time, const ::std::string& iceP_extension, const ::Ice::Context& context = ::Ice::noExplicitContext)
    {
        return _iceI_begin_writeCheckInfo(iceP_uVSSImageName, iceP_uVSSImage, iceP_plateImageName, iceP_plateImage, iceP_channel, iceP_plateNumber, iceP_direction, iceP_time, iceP_extension, context, ::IceInternal::dummyCallback, 0);
    }

    ::Ice::AsyncResultPtr begin_writeCheckInfo(const ::std::string& iceP_uVSSImageName, const ::UVSS::ByteSeq& iceP_uVSSImage, const ::std::string& iceP_plateImageName, const ::UVSS::ByteSeq& iceP_plateImage, const ::std::string& iceP_channel, const ::std::string& iceP_plateNumber, const ::std::string& iceP_direction, const ::std::string& iceP_time, const ::std::string& iceP_extension, const ::Ice::CallbackPtr& del, const ::Ice::LocalObjectPtr& cookie = 0)
    {
        return _iceI_begin_writeCheckInfo(iceP_uVSSImageName, iceP_uVSSImage, iceP_plateImageName, iceP_plateImage, iceP_channel, iceP_plateNumber, iceP_direction, iceP_time, iceP_extension, ::Ice::noExplicitContext, del, cookie);
    }

    ::Ice::AsyncResultPtr begin_writeCheckInfo(const ::std::string& iceP_uVSSImageName, const ::UVSS::ByteSeq& iceP_uVSSImage, const ::std::string& iceP_plateImageName, const ::UVSS::ByteSeq& iceP_plateImage, const ::std::string& iceP_channel, const ::std::string& iceP_plateNumber, const ::std::string& iceP_direction, const ::std::string& iceP_time, const ::std::string& iceP_extension, const ::Ice::Context& context, const ::Ice::CallbackPtr& del, const ::Ice::LocalObjectPtr& cookie = 0)
    {
        return _iceI_begin_writeCheckInfo(iceP_uVSSImageName, iceP_uVSSImage, iceP_plateImageName, iceP_plateImage, iceP_channel, iceP_plateNumber, iceP_direction, iceP_time, iceP_extension, context, del, cookie);
    }

    ::Ice::AsyncResultPtr begin_writeCheckInfo(const ::std::string& iceP_uVSSImageName, const ::UVSS::ByteSeq& iceP_uVSSImage, const ::std::string& iceP_plateImageName, const ::UVSS::ByteSeq& iceP_plateImage, const ::std::string& iceP_channel, const ::std::string& iceP_plateNumber, const ::std::string& iceP_direction, const ::std::string& iceP_time, const ::std::string& iceP_extension, const ::UVSS::Callback_Client_writeCheckInfoPtr& del, const ::Ice::LocalObjectPtr& cookie = 0)
    {
        return _iceI_begin_writeCheckInfo(iceP_uVSSImageName, iceP_uVSSImage, iceP_plateImageName, iceP_plateImage, iceP_channel, iceP_plateNumber, iceP_direction, iceP_time, iceP_extension, ::Ice::noExplicitContext, del, cookie);
    }

    ::Ice::AsyncResultPtr begin_writeCheckInfo(const ::std::string& iceP_uVSSImageName, const ::UVSS::ByteSeq& iceP_uVSSImage, const ::std::string& iceP_plateImageName, const ::UVSS::ByteSeq& iceP_plateImage, const ::std::string& iceP_channel, const ::std::string& iceP_plateNumber, const ::std::string& iceP_direction, const ::std::string& iceP_time, const ::std::string& iceP_extension, const ::Ice::Context& context, const ::UVSS::Callback_Client_writeCheckInfoPtr& del, const ::Ice::LocalObjectPtr& cookie = 0)
    {
        return _iceI_begin_writeCheckInfo(iceP_uVSSImageName, iceP_uVSSImage, iceP_plateImageName, iceP_plateImage, iceP_channel, iceP_plateNumber, iceP_direction, iceP_time, iceP_extension, context, del, cookie);
    }

    void end_writeCheckInfo(const ::Ice::AsyncResultPtr&);
    
private:

    ::Ice::AsyncResultPtr _iceI_begin_writeCheckInfo(const ::std::string&, const ::UVSS::ByteSeq&, const ::std::string&, const ::UVSS::ByteSeq&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&, const ::IceInternal::CallbackBasePtr&, const ::Ice::LocalObjectPtr& cookie = 0, bool sync = false);
    
public:
    
    static const ::std::string& ice_staticId();

protected: 

    virtual ::IceProxy::Ice::Object* _newInstance() const;
};

class Server : public virtual ::Ice::Proxy<Server, ::IceProxy::Ice::Object>
{
public:

    bool checkVersion(const ::std::string& iceP_ver, const ::Ice::Context& context = ::Ice::noExplicitContext)
    {
        return end_checkVersion(_iceI_begin_checkVersion(iceP_ver, context, ::IceInternal::dummyCallback, 0, true));
    }

    ::Ice::AsyncResultPtr begin_checkVersion(const ::std::string& iceP_ver, const ::Ice::Context& context = ::Ice::noExplicitContext)
    {
        return _iceI_begin_checkVersion(iceP_ver, context, ::IceInternal::dummyCallback, 0);
    }

    ::Ice::AsyncResultPtr begin_checkVersion(const ::std::string& iceP_ver, const ::Ice::CallbackPtr& del, const ::Ice::LocalObjectPtr& cookie = 0)
    {
        return _iceI_begin_checkVersion(iceP_ver, ::Ice::noExplicitContext, del, cookie);
    }

    ::Ice::AsyncResultPtr begin_checkVersion(const ::std::string& iceP_ver, const ::Ice::Context& context, const ::Ice::CallbackPtr& del, const ::Ice::LocalObjectPtr& cookie = 0)
    {
        return _iceI_begin_checkVersion(iceP_ver, context, del, cookie);
    }

    ::Ice::AsyncResultPtr begin_checkVersion(const ::std::string& iceP_ver, const ::UVSS::Callback_Server_checkVersionPtr& del, const ::Ice::LocalObjectPtr& cookie = 0)
    {
        return _iceI_begin_checkVersion(iceP_ver, ::Ice::noExplicitContext, del, cookie);
    }

    ::Ice::AsyncResultPtr begin_checkVersion(const ::std::string& iceP_ver, const ::Ice::Context& context, const ::UVSS::Callback_Server_checkVersionPtr& del, const ::Ice::LocalObjectPtr& cookie = 0)
    {
        return _iceI_begin_checkVersion(iceP_ver, context, del, cookie);
    }

    bool end_checkVersion(const ::Ice::AsyncResultPtr&);
    
private:

    ::Ice::AsyncResultPtr _iceI_begin_checkVersion(const ::std::string&, const ::Ice::Context&, const ::IceInternal::CallbackBasePtr&, const ::Ice::LocalObjectPtr& cookie = 0, bool sync = false);
    
public:

    void addClient(const ::Ice::Identity& iceP_id, const ::Ice::Context& context = ::Ice::noExplicitContext)
    {
        end_addClient(_iceI_begin_addClient(iceP_id, context, ::IceInternal::dummyCallback, 0, true));
    }

    ::Ice::AsyncResultPtr begin_addClient(const ::Ice::Identity& iceP_id, const ::Ice::Context& context = ::Ice::noExplicitContext)
    {
        return _iceI_begin_addClient(iceP_id, context, ::IceInternal::dummyCallback, 0);
    }

    ::Ice::AsyncResultPtr begin_addClient(const ::Ice::Identity& iceP_id, const ::Ice::CallbackPtr& del, const ::Ice::LocalObjectPtr& cookie = 0)
    {
        return _iceI_begin_addClient(iceP_id, ::Ice::noExplicitContext, del, cookie);
    }

    ::Ice::AsyncResultPtr begin_addClient(const ::Ice::Identity& iceP_id, const ::Ice::Context& context, const ::Ice::CallbackPtr& del, const ::Ice::LocalObjectPtr& cookie = 0)
    {
        return _iceI_begin_addClient(iceP_id, context, del, cookie);
    }

    ::Ice::AsyncResultPtr begin_addClient(const ::Ice::Identity& iceP_id, const ::UVSS::Callback_Server_addClientPtr& del, const ::Ice::LocalObjectPtr& cookie = 0)
    {
        return _iceI_begin_addClient(iceP_id, ::Ice::noExplicitContext, del, cookie);
    }

    ::Ice::AsyncResultPtr begin_addClient(const ::Ice::Identity& iceP_id, const ::Ice::Context& context, const ::UVSS::Callback_Server_addClientPtr& del, const ::Ice::LocalObjectPtr& cookie = 0)
    {
        return _iceI_begin_addClient(iceP_id, context, del, cookie);
    }

    void end_addClient(const ::Ice::AsyncResultPtr&);
    
private:

    ::Ice::AsyncResultPtr _iceI_begin_addClient(const ::Ice::Identity&, const ::Ice::Context&, const ::IceInternal::CallbackBasePtr&, const ::Ice::LocalObjectPtr& cookie = 0, bool sync = false);
    
public:
    
    static const ::std::string& ice_staticId();

protected: 

    virtual ::IceProxy::Ice::Object* _newInstance() const;
};

}

}

namespace UVSS
{

class Client : public virtual ::Ice::Object
{
public:

    typedef ClientPrx ProxyType;
    typedef ClientPtr PointerType;

    virtual ~Client();

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::noExplicitCurrent) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::noExplicitCurrent) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::noExplicitCurrent) const;

    static const ::std::string& ice_staticId();

    virtual void writeCheckInfo_async(const ::UVSS::AMD_Client_writeCheckInfoPtr&, const ::std::string&, const ::UVSS::ByteSeq&, const ::std::string&, const ::UVSS::ByteSeq&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::noExplicitCurrent) = 0;
    bool _iceD_writeCheckInfo(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool _iceDispatch(::IceInternal::Incoming&, const ::Ice::Current&);

protected:

    virtual void _iceWriteImpl(::Ice::OutputStream*) const;
    virtual void _iceReadImpl(::Ice::InputStream*);
};

inline bool operator==(const Client& lhs, const Client& rhs)
{
    return static_cast<const ::Ice::Object&>(lhs) == static_cast<const ::Ice::Object&>(rhs);
}

inline bool operator<(const Client& lhs, const Client& rhs)
{
    return static_cast<const ::Ice::Object&>(lhs) < static_cast<const ::Ice::Object&>(rhs);
}

class Server : public virtual ::Ice::Object
{
public:

    typedef ServerPrx ProxyType;
    typedef ServerPtr PointerType;

    virtual ~Server();

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::noExplicitCurrent) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::noExplicitCurrent) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::noExplicitCurrent) const;

    static const ::std::string& ice_staticId();

    virtual bool checkVersion(const ::std::string&, const ::Ice::Current& = ::Ice::noExplicitCurrent) = 0;
    bool _iceD_checkVersion(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void addClient(const ::Ice::Identity&, const ::Ice::Current& = ::Ice::noExplicitCurrent) = 0;
    bool _iceD_addClient(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool _iceDispatch(::IceInternal::Incoming&, const ::Ice::Current&);

protected:

    virtual void _iceWriteImpl(::Ice::OutputStream*) const;
    virtual void _iceReadImpl(::Ice::InputStream*);
};

inline bool operator==(const Server& lhs, const Server& rhs)
{
    return static_cast<const ::Ice::Object&>(lhs) == static_cast<const ::Ice::Object&>(rhs);
}

inline bool operator<(const Server& lhs, const Server& rhs)
{
    return static_cast<const ::Ice::Object&>(lhs) < static_cast<const ::Ice::Object&>(rhs);
}

}

namespace Ice
{

template<>
struct StreamableTraits< ::UVSS::RequestCanceledException>
{
    static const StreamHelperCategory helper = StreamHelperCategoryUserException;
};

}

namespace UVSS
{

template<class T>
class CallbackNC_Client_writeCheckInfo : public Callback_Client_writeCheckInfo_Base, public ::IceInternal::TwowayCallbackNC<T>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception&);
    typedef void (T::*Sent)(bool);
    typedef void (T::*Response)();

    CallbackNC_Client_writeCheckInfo(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::TwowayCallbackNC<T>(obj, cb != 0, excb, sentcb), _response(cb)
    {
    }

    virtual void completed(const ::Ice::AsyncResultPtr& result) const
    {
        ::UVSS::ClientPrx proxy = ::UVSS::ClientPrx::uncheckedCast(result->getProxy());
        try
        {
            proxy->end_writeCheckInfo(result);
        }
        catch(const ::Ice::Exception& ex)
        {
            ::IceInternal::CallbackNC<T>::exception(result, ex);
            return;
        }
        if(_response)
        {
            (::IceInternal::CallbackNC<T>::_callback.get()->*_response)();
        }
    }

private:

    Response _response;
};

template<class T> Callback_Client_writeCheckInfoPtr
newCallback_Client_writeCheckInfo(const IceUtil::Handle<T>& instance, void (T::*cb)(), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_Client_writeCheckInfo<T>(instance, cb, excb, sentcb);
}

template<class T> Callback_Client_writeCheckInfoPtr
newCallback_Client_writeCheckInfo(const IceUtil::Handle<T>& instance, void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_Client_writeCheckInfo<T>(instance, 0, excb, sentcb);
}

template<class T> Callback_Client_writeCheckInfoPtr
newCallback_Client_writeCheckInfo(T* instance, void (T::*cb)(), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_Client_writeCheckInfo<T>(instance, cb, excb, sentcb);
}

template<class T> Callback_Client_writeCheckInfoPtr
newCallback_Client_writeCheckInfo(T* instance, void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_Client_writeCheckInfo<T>(instance, 0, excb, sentcb);
}

template<class T, typename CT>
class Callback_Client_writeCheckInfo : public Callback_Client_writeCheckInfo_Base, public ::IceInternal::TwowayCallback<T, CT>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception& , const CT&);
    typedef void (T::*Sent)(bool , const CT&);
    typedef void (T::*Response)(const CT&);

    Callback_Client_writeCheckInfo(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::TwowayCallback<T, CT>(obj, cb != 0, excb, sentcb), _response(cb)
    {
    }

    virtual void completed(const ::Ice::AsyncResultPtr& result) const
    {
        ::UVSS::ClientPrx proxy = ::UVSS::ClientPrx::uncheckedCast(result->getProxy());
        try
        {
            proxy->end_writeCheckInfo(result);
        }
        catch(const ::Ice::Exception& ex)
        {
            ::IceInternal::Callback<T, CT>::exception(result, ex);
            return;
        }
        if(_response)
        {
            (::IceInternal::Callback<T, CT>::_callback.get()->*_response)(CT::dynamicCast(result->getCookie()));
        }
    }

private:

    Response _response;
};

template<class T, typename CT> Callback_Client_writeCheckInfoPtr
newCallback_Client_writeCheckInfo(const IceUtil::Handle<T>& instance, void (T::*cb)(const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_Client_writeCheckInfo<T, CT>(instance, cb, excb, sentcb);
}

template<class T, typename CT> Callback_Client_writeCheckInfoPtr
newCallback_Client_writeCheckInfo(const IceUtil::Handle<T>& instance, void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_Client_writeCheckInfo<T, CT>(instance, 0, excb, sentcb);
}

template<class T, typename CT> Callback_Client_writeCheckInfoPtr
newCallback_Client_writeCheckInfo(T* instance, void (T::*cb)(const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_Client_writeCheckInfo<T, CT>(instance, cb, excb, sentcb);
}

template<class T, typename CT> Callback_Client_writeCheckInfoPtr
newCallback_Client_writeCheckInfo(T* instance, void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_Client_writeCheckInfo<T, CT>(instance, 0, excb, sentcb);
}

template<class T>
class CallbackNC_Server_checkVersion : public Callback_Server_checkVersion_Base, public ::IceInternal::TwowayCallbackNC<T>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception&);
    typedef void (T::*Sent)(bool);
    typedef void (T::*Response)(bool);

    CallbackNC_Server_checkVersion(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::TwowayCallbackNC<T>(obj, cb != 0, excb, sentcb), _response(cb)
    {
    }

    virtual void completed(const ::Ice::AsyncResultPtr& result) const
    {
        ::UVSS::ServerPrx proxy = ::UVSS::ServerPrx::uncheckedCast(result->getProxy());
        bool ret;
        try
        {
            ret = proxy->end_checkVersion(result);
        }
        catch(const ::Ice::Exception& ex)
        {
            ::IceInternal::CallbackNC<T>::exception(result, ex);
            return;
        }
        if(_response)
        {
            (::IceInternal::CallbackNC<T>::_callback.get()->*_response)(ret);
        }
    }

private:

    Response _response;
};

template<class T> Callback_Server_checkVersionPtr
newCallback_Server_checkVersion(const IceUtil::Handle<T>& instance, void (T::*cb)(bool), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_Server_checkVersion<T>(instance, cb, excb, sentcb);
}

template<class T> Callback_Server_checkVersionPtr
newCallback_Server_checkVersion(T* instance, void (T::*cb)(bool), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_Server_checkVersion<T>(instance, cb, excb, sentcb);
}

template<class T, typename CT>
class Callback_Server_checkVersion : public Callback_Server_checkVersion_Base, public ::IceInternal::TwowayCallback<T, CT>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception& , const CT&);
    typedef void (T::*Sent)(bool , const CT&);
    typedef void (T::*Response)(bool, const CT&);

    Callback_Server_checkVersion(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::TwowayCallback<T, CT>(obj, cb != 0, excb, sentcb), _response(cb)
    {
    }

    virtual void completed(const ::Ice::AsyncResultPtr& result) const
    {
        ::UVSS::ServerPrx proxy = ::UVSS::ServerPrx::uncheckedCast(result->getProxy());
        bool ret;
        try
        {
            ret = proxy->end_checkVersion(result);
        }
        catch(const ::Ice::Exception& ex)
        {
            ::IceInternal::Callback<T, CT>::exception(result, ex);
            return;
        }
        if(_response)
        {
            (::IceInternal::Callback<T, CT>::_callback.get()->*_response)(ret, CT::dynamicCast(result->getCookie()));
        }
    }

private:

    Response _response;
};

template<class T, typename CT> Callback_Server_checkVersionPtr
newCallback_Server_checkVersion(const IceUtil::Handle<T>& instance, void (T::*cb)(bool, const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_Server_checkVersion<T, CT>(instance, cb, excb, sentcb);
}

template<class T, typename CT> Callback_Server_checkVersionPtr
newCallback_Server_checkVersion(T* instance, void (T::*cb)(bool, const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_Server_checkVersion<T, CT>(instance, cb, excb, sentcb);
}

template<class T>
class CallbackNC_Server_addClient : public Callback_Server_addClient_Base, public ::IceInternal::OnewayCallbackNC<T>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception&);
    typedef void (T::*Sent)(bool);
    typedef void (T::*Response)();

    CallbackNC_Server_addClient(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::OnewayCallbackNC<T>(obj, cb, excb, sentcb)
    {
    }
};

template<class T> Callback_Server_addClientPtr
newCallback_Server_addClient(const IceUtil::Handle<T>& instance, void (T::*cb)(), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_Server_addClient<T>(instance, cb, excb, sentcb);
}

template<class T> Callback_Server_addClientPtr
newCallback_Server_addClient(const IceUtil::Handle<T>& instance, void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_Server_addClient<T>(instance, 0, excb, sentcb);
}

template<class T> Callback_Server_addClientPtr
newCallback_Server_addClient(T* instance, void (T::*cb)(), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_Server_addClient<T>(instance, cb, excb, sentcb);
}

template<class T> Callback_Server_addClientPtr
newCallback_Server_addClient(T* instance, void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_Server_addClient<T>(instance, 0, excb, sentcb);
}

template<class T, typename CT>
class Callback_Server_addClient : public Callback_Server_addClient_Base, public ::IceInternal::OnewayCallback<T, CT>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception& , const CT&);
    typedef void (T::*Sent)(bool , const CT&);
    typedef void (T::*Response)(const CT&);

    Callback_Server_addClient(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::OnewayCallback<T, CT>(obj, cb, excb, sentcb)
    {
    }
};

template<class T, typename CT> Callback_Server_addClientPtr
newCallback_Server_addClient(const IceUtil::Handle<T>& instance, void (T::*cb)(const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_Server_addClient<T, CT>(instance, cb, excb, sentcb);
}

template<class T, typename CT> Callback_Server_addClientPtr
newCallback_Server_addClient(const IceUtil::Handle<T>& instance, void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_Server_addClient<T, CT>(instance, 0, excb, sentcb);
}

template<class T, typename CT> Callback_Server_addClientPtr
newCallback_Server_addClient(T* instance, void (T::*cb)(const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_Server_addClient<T, CT>(instance, cb, excb, sentcb);
}

template<class T, typename CT> Callback_Server_addClientPtr
newCallback_Server_addClient(T* instance, void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_Server_addClient<T, CT>(instance, 0, excb, sentcb);
}

}

#endif

#include <IceUtil/PopDisableWarnings.h>
#endif
