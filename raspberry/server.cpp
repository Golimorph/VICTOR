#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <wiringPiI2C.h>
#define DEVICE_ID 0x09 //This is the device ID of the arduino, same number written on the arduino side.

#define PORT 8080


int sendI2CMessageToArduino (uint8_t data)
{



    return 0;
}





int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Setup I2C communication
    int fd = wiringPiI2CSetup(DEVICE_ID);
    if (fd == -1)
    {
        std::cout << "Failed to init I2C communication.\n";
        return -1;
    }
    std::cout << "I2C communication successfully setup.\n";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }


    while(true)
    {
        int valread = read(new_socket, buffer, 1024);
        std::cout << "Message received: " << buffer << std::endl;

        // Create a string to send back to the client
        std::string messageToClient = "Hello from Raspberry Pi, I got this number: ";
        std::string numberString = std::string(buffer, valread);
        messageToClient += numberString;  // Convert buffer to string and append

        uint8_t data = static_cast <uint8_t>(std::stoi(numberString));
        //wiringPiI2CWrite(fd, data);
        std::cout << "Sent data: " << std::to_string(data) << "\n";
    }


    close(new_socket);
    close(server_fd);
    return 0;
}
