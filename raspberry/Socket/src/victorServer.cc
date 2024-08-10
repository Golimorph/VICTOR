#include "victorServer.h"

VictorServer::VictorServer(CallbackType callback)
{	
	m_callback = callback;
	if(initializeSocket())
	{
		std::cout<<"Socket between victor and android/desktop device initialized successfully!\n";
	}
}


VictorServer::~VictorServer()
{
	close(m_socket);
    close(m_server_fd);
}


std::vector<std::string> VictorServer::splitString(const std::string &str) {
    std::vector<std::string> substrings;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, ';')) {
        substrings.push_back(item);
    }

    return substrings;
}

void VictorServer::startI2Cforwarding()
{
	char buffer[1024] = {0};
	while(true)
    {
        int valread = read(m_socket, buffer, 1024);

        if (valread <= 0) 
        {
        	if(valread==0)
        	{
        		std::cerr<<"Socket was closed by client, will try to open a new socket, waiting for client.\n";
        	}else
        	{
        		std::cerr<<"Socket connection lost due to error, will try to open a new socket, waiting for client.\n";
        	}
        	if(initializeSocket())
			{
				std::cerr<<"New socket between victor and client initialized successfully!\n";
			}
        	valread = read(m_socket, buffer, 1024);
        }

        std::vector<std::string> messages = splitString(std::string(buffer, valread));
        for(const auto &message : messages)
        {
        	/*Sometimes several messages are sent in the same buffer, so need to split them and send them
        	separately over I2C. Hence this for loop. Instead of sending the buffer directly.*/
            m_callback(message);
    	}
    }
}

bool VictorServer::initializeSocket()
{
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
 
    // Creating socket file descriptor
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr<<"socket failed\n";
        return false;
    }
    
    // Forcefully attaching socket to the port 8080
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr<<"setsockopt\n";
        return false;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Forcefully attaching socket to the port 8080
    if (bind(m_server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr<<"bind failed\n";
        return false;
    }
    if (listen(m_server_fd, 3) < 0) {
        std::cerr<<"listen\n";
        return false;
    }
    if ((m_socket = accept(m_server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr<<"accept\n";
        return false;
    }

    return true;
}







