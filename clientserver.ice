#ifndef CLIENTSERVER_ICE
#define CLIENTSERVER_ICE

#include <Ice/Identity.ice>

module UVSS {
sequence<byte> ByteSeq;

interface Client {
	void writeCheckInfo(
		string uVSSImageName, ByteSeq uVSSImage,
		string plateImageName, ByteSeq plateImage,
		string channel,
		string plateNumber,
		string direction,
		string checkDateTime,
		string extension);
};

interface Server {
	void addClient(Ice::Identity ident);
	bool checkVersion(string ver);
};
};

#endif
