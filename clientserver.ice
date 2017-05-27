#pragma once

#include <Ice/Identity.ice>

module UVSS {
sequence<byte> ByteSeq;
sequence<string> StringSeq;

exception RequestCanceledException {
}

interface Client {
    ["amd"] idempotent void writeCheckInfo(
            string uVSSImageName, ByteSeq uVSSImage,
            string plateImageName, ByteSeq plateImage,
            StringSeq ss)
            throws RequestCanceledException;
}

interface Server {
    bool checkVersion(string ver);
    void addClient(Ice::Identity id);
}
}

