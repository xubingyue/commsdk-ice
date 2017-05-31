#pragma once

#include <Ice/Identity.ice>

module UVSS {
sequence<byte> ByteSeq;
sequence<string> StringSeq;
sequence<ByteSeq> ByteSeqSeq;

exception RequestCanceledException {
}

interface Client {
    ["amd"] idempotent void writeCheckInfo(
            StringSeq ns, ByteSeqSeq bss,
            StringSeq ss)
            throws RequestCanceledException;
}

interface Server {
    bool checkVersion(string ver);
    void addClient(Ice::Identity id);
}
}

