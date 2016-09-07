#ifndef CLIENTSERVER_ICE
#define CLIENTSERVER_ICE

#include <Ice/Identity.ice>

module UVSS {
sequence<byte> ByteSeq;

interface Client {
	string readClientUVSSImagePath();
	string readClientPlateImagePath();
	
	void writeClientUVSSImagePath(string clientUVSSImagePath, ByteSeq serverUVSSImage);
	void writeClientPlateImagePath(string clientPlateImagePath, ByteSeq serverPlateImage);
	
	void writeClientUVSSImageRelativePath(string serverUVSSImageRelativePath);
	void writeClientPlateImageRelativePath(string serverPlateImageRelativePath);
	
	void writeClientChannel(string serverChannel);
	void writeClientPlateNumber(string serverPlateNumber);
	void writeClientDirection(string serverDirection);
	void writeClientCheckDateTime(string serverCheckDateTime);
	void writeClientExtension(string serverExtension);
	
	void createClientImageDirectory();
	
	void useClientCheckInfoCallback();
};

interface Server {
	//void addClient(Client* clientProxy);
	void addClient(Ice::Identity ident);
	void shutdown();
	
	void useServerConnectionInfoCallback(int type, string clientConnectionInfo);
};
};

#endif
