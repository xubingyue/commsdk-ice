#pragma once

#include <Ice/Identity.ice>

module UVSS {
sequence<string> StringSeq;
sequence<byte> ByteSeq;
sequence<ByteSeq> ByteSeqSeq;

exception RequestCanceledException {
}

interface CallbackReceiver {
    ["amd"] idempotent void sendCheckInfo(
        StringSeq filenames, ByteSeqSeq files,
        StringSeq strings)
        throws RequestCanceledException;
}

interface CallbackSender {
    bool checkVersion(string version);
    void addClient(Ice::Identity ident);
}
}
