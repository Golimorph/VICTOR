#pragma once
#include "victorUart.h"
#include "camera.h"

/*! @brief This class contains the neccesary program to approach an object that has been detected
 * by one of victors sensors. It takes input from victors Camera and other sensors computes the
 * best path to the target and send the information over UART to the Arduino for execution of motor
 * functions. */
class Approach
{
public:
    Approach(VictorUart& victorUart, Camera& camera, std::string object);

    /*!@ Start the approach to the target object.
     * @return true if the object has been succesfully, false if the object has been lost,
     *  	   victor is stuck or abort has been called.*/
    bool execute();

    /*!@ Abort the ongoing approach.*/
    void abort();

private:
    int8_t clampToInt8(int value);

    VictorUart& m_victorUart;
    Camera& m_camera;
    std::string m_object;
};