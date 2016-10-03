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
			string time,
			string extension);
};

interface Server {
	bool checkVersion(string ver);
	void addClient(Ice::Identity id);
};
};

#endif
