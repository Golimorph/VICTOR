#pragma once


#include <string>
#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <functional>
#include <sstream>
#include <wiringPi.h>

#define PORT 8080

class VictorServer
{
public:
	using CallbackType = std::function<bool(std::string)>;
	VictorServer(CallbackType callback);
	~VictorServer();
	void startI2Cforwarding();
private:
	CallbackType m_i2cCallback;
	bool initializeSocket();
	void turnOnHeadLights();
	std::vector<std::string> splitString(const std::string &str);
	int m_socket;
    int m_server_fd;
};
