#pragma once

#include <Ice/Identity.ice>

module Uvss {
sequence<string> StringSeq;
sequence<byte> ByteSeq;
sequence<ByteSeq> ByteSeqSeq;

const string version = "1.0";

exception RequestCanceledException {
}

interface CallbackReceiver {
    ["amd"] idempotent void sendData(
        StringSeq strings,
        StringSeq fileNames, ByteSeqSeq files)
        throws RequestCanceledException;
}

interface CallbackSender {
    bool checkVersion(string version);
    void addProxy(Ice::Identity ident);
    ["amd"] idempotent void sendData(
        StringSeq strings,
        StringSeq fileNames, ByteSeqSeq files)
        throws RequestCanceledException;
}
}
