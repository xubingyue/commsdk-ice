#ifndef CLIENTSERVER_ICE
#define CLIENTSERVER_ICE

#include <Ice/Identity.ice>

module UVSS {
sequence<byte> ByteSeq;

interface Client {
	void writeCheckInfo(
		string clientUVSSImagePath, ByteSeq serverUVSSImage,
		string clientPlateImagePath, ByteSeq serverPlateImage,
		string serverChannel,
		string serverPlateNumber,
		string serverDirection,
		string serverCheckDateTime,
		string serverExtension
		);

	void createClientImageDirectory(string clientImageDirectory);
	void useClientCheckInfoCallback();

	void heartBeat();
};

interface Server {
	void addClient(Ice::Identity ident);
	void useServerConnectionInfoCallback(int type, string clientConnectionInfo);
	void heartBeat();

	bool checkVersion(string ver);
};
};

#endif
