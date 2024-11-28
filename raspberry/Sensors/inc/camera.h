#pragma once
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <cstdlib>
#include <thread>

/*! @brief The camera class is an intermediary to the Hailo python API and receives messages sent
 * from the code in the python folder. The messages typically contain informaton about objects such
 * as type, size, position of objects.*/
class Camera
{
public:
    explicit Camera();

    /*! Get the position of all objects of a certain type currently detected by the camera
     * @param object, what object to look for
     * @param return x,y coordinates from 0 to 1 with the position of the object.*/
    bool getDetections(std::string object);

private:
    void startHailo();
    void runHailo();

    int m_server_fd;
    int m_socket;
    std::thread m_hailoThread;
};


