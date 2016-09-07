#ifndef CLIENTSERVER_ICE
#define CLIENTSERVER_ICE

#include <Ice/Identity.ice>

module UVSS {
sequence<byte> ByteSeq;

interface Client {
	void writeClientUVSSImagePath(string clientUVSSImagePath, ByteSeq serverUVSSImage);
	void writeClientPlateImagePath(string clientPlateImagePath, ByteSeq serverPlateImage);
	
	void writeClientUVSSImageRelativePath(string serverUVSSImageRelativePath);
	void writeClientPlateImageRelativePath(string serverPlateImageRelativePath);
	
	void writeClientChannel(string serverChannel);
	void writeClientPlateNumber(string serverPlateNumber);
	void writeClientDirection(string serverDirection);
	void writeClientCheckDateTime(string serverCheckDateTime);
	void writeClientExtension(string serverExtension);
	
	void createClientImageDirectory(string clientImageDirectory);
	void useClientCheckInfoCallback();
	void heartBeat();
};

interface Server {
	void addClient(Ice::Identity ident);
	void useServerConnectionInfoCallback(int type, string clientConnectionInfo);
	void heartBeat();
};
};

#endif
