#pragma once
class Response
{
public:
	char* msg;
	char* destinationIP;
	int destinationPort;
	Response();
	~Response();
};

