#pragma once

#include <Ice/Identity.ice>

module UVSS {
sequence<byte> ByteSeq;

exception RequestCanceledException
{
};

interface Client {
    ["amd"] idempotent void writeCheckInfo(
            string uVSSImageName, ByteSeq uVSSImage,
            string plateImageName, ByteSeq plateImage,
            string channel,
            string plateNumber,
            string direction,
            string time,
            string extension)
	throws RequestCanceledException;
};

interface Server {
    bool checkVersion(string ver);
    void addClient(Ice::Identity id);
};
};

