#include "camera.h"


Camera::Camera()
{
    //Create a socket from the cpp program to the python program
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

    if (listen(m_server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    startHailo();

    if ((m_socket = accept(m_server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

}


void Camera::startHailo()
{
    m_hailoThread = std::thread(&Camera::runHailo, this);
}

void Camera::runHailo()
{
    INFO("Raspberry: launching Hailo");
    const char* command = R"(
        bash -c '
        cd /home/victor/Repository/Python &&
        source setup_env.sh &&
        python detection.py --labels-json barcode-labels.json --hef barcode.hef -i rpi > /dev/null 2>&1')";
    //python detection.py --labels-json resources/barcode-labels.json --hef resources/yolov8s-hailo8l-barcode.hef --input /dev/video0 > /dev/null 2>&1
    //python detection.py --input /dev/video0 > /dev/null 2>&1

    if (std::system(command) != 0)
    {
        ERROR("Raspberrypi: Error starting Hailo.");
    }
}


// Function to extract object properties from the string
Camera::ObjectProperties Camera::parseObjectProperties(const std::string& input)
{
    ObjectProperties objProps;

    // Use regular expressions to extract object type and numbers
    std::regex number_regex(R"([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)");
    std::regex type_regex(R"([a-zA-Z]+)");  // Matches the type (letters only)

    // First, extract the object type (e.g., "mouse")
    std::smatch type_match;
    if (std::regex_search(input, type_match, type_regex))
    {
        objProps.type = type_match.str();
    }

    // Extract numbers using regex
    std::sregex_iterator it(input.begin(), input.end(), number_regex);
    std::sregex_iterator end;

    // Extract the numbers and fill the struct
    int index = 0;
    while (it != end && index < 5)
    {
        double value = std::stod(it->str());
        switch (index)
        {
        case 0:
            objProps.probability = value;
            break;
        case 1:
            objProps.xmin = value;
            break;
        case 2:
            objProps.xmax = value;
            break;
        case 3:
            objProps.ymin = value;
            break;
        case 4:
            objProps.ymax = value;
            break;
        }
        ++it;
        ++index;
    }

    return objProps;
}

std::optional<Camera::ObjectProperties> Camera::getDetection(std::string object)
{
    //TODO make it so that object is sent to the python program and the c++ program will wait for the returned message from the callback.

    //write(m_socket, object.c_str(), object.size());//Tell Hailo what object I want to detect.
    char buffer[1024] = {0};
    int valread = read(m_socket, buffer, 1024);
    if (valread > 0)
    {
        //std::cerr << "Raspberry: Received message from Hailo: " << buffer << std::endl;
        return parseObjectProperties(buffer); //object found.
    }
    return std::nullopt; //object not found
}









