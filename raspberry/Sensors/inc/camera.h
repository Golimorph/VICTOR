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
#include <regex>
#include <optional>

/*! @brief The camera class is an intermediary to the Hailo python API and receives messages sent
 * from the code in the python folder. The messages typically contain informaton about objects such
 * as type, size, position of objects.*/
class Camera
{
public:
    explicit Camera();

    //The properties of a detected object. Location and type.
	struct ObjectProperties {
		std::string type;
		double probability;
		double xmin;
		double xmax;
		double ymin;
		double ymax;
	};

    /*! Get the position of the first object of a certain type currently detected by the camera
     * @param object, what object to look for
     * @param return object detected, probablility, xmin,xmax,ymin,ymax coordinates from 0 to 1 with the position of the object.*/
    std::optional<ObjectProperties> getDetection(std::string object);

private:
    void startHailo();
    void runHailo();
    ObjectProperties parseObjectProperties(const std::string& input);

    int m_server_fd;
    int m_socket;
    std::thread m_hailoThread;
};


