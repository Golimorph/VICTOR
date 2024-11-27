#include "camera.h"


Camera::Camera()
{
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8081
    int opt = 1;
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) 
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8081);

    // Forcefully attaching socket to the port 8081
    if (bind(m_server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(m_server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((m_socket = accept(m_server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

}



bool Camera::getDetections(std::string object) 
{
	//TODO make it so that object is sent to the python program and the c++ program will wait for the returned message from the callback.
	//write(socket, solution.data(), solution.size() * sizeof(double));
    char buffer[1024] = {0};
    int valread = read(m_socket, buffer, 1024);
    if (valread > 0) 
    {
        std::cerr << "Raspberry: Received message from Hailo: " << buffer << std::endl;
        return true;
    }
    return false;
    //std::vector<std::vector<double>> result;
    //return result;
}









