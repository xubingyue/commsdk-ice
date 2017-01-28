
#include <clientserverI.h>

void
::UVSS::ClientI::writeCheckInfoAsync(::std::string uVSSImageName,
                                     ::UVSS::ByteSeq uVSSImage,
                                     ::std::string plateImageName,
                                     ::UVSS::ByteSeq plateImage,
                                     ::std::string channel,
                                     ::std::string plateNumber,
                                     ::std::string direction,
                                     ::std::string time,
                                     ::std::string extension,
                                     std::function<void()> writeCheckInfo_response,
                                     std::function<void(std::exception_ptr)>,
                                     const Ice::Current& current)
{
    writeCheckInfo_response();
}

void
UVSS::ServerI::addClient(::Ice::Identity /*id*/,
                         const Ice::Current& current)
{
}

bool
UVSS::ServerI::checkVersion(::std::string /*ver*/,
                            const Ice::Current& current)
{
    return false;
}
