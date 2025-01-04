#include "approach.h"

Approach::Approach(VictorUart& victorUart, Camera& camera, std::string object)
    :m_victorUart(victorUart), m_camera(camera), m_object(object)
{
}



int8_t Approach::clampToInt8(int value)
{
    if (value > 127)
    {
        return 127; // Maximum value for int8_t
    }
    else if (value < -127)
    {
        return -127; // Minimum value in your desired range
    }
    return static_cast<int8_t>(value); // Cast safely if within range
}

bool Approach::execute()
{
	bool arrived = false;
	bool done = false;
    while(!done)
    {

        auto detection = m_camera.getDetection(m_object);
        if(detection.has_value())
        {
        	double bottleSize = (detection.value().xmax - detection.value().xmin);


        	if(bottleSize < 0.13)
        	{
        		arrived = false;
        	}

        	if(bottleSize > 0.15)
        	{
        		arrived = true;
        	}

        	if(arrived == false)
        	{
	        	std::cerr << "In execute: " << detection.value().type << ": " << detection.value().probability <<"\n";
	            int compValue = static_cast<int>(1000*bottleSize*((detection.value().xmin + detection.value().xmax)/2 - 0.5));
	            raspberryIf::MoveTracksMessage moveTracksMessage;
	            moveTracksMessage.leftTrackSpeed = clampToInt8(6/bottleSize + compValue);
	            moveTracksMessage.rightTrackSpeed = clampToInt8(6/bottleSize - compValue);
	            m_victorUart.doMoveTracks(moveTracksMessage);
	        }else
	        {
	        	raspberryIf::MoveTracksMessage moveTracksMessage;
	            moveTracksMessage.leftTrackSpeed = 30;
	            moveTracksMessage.rightTrackSpeed = 30;
	            m_victorUart.doMoveTracks(moveTracksMessage);
	            sleep(2);
	            moveTracksMessage.leftTrackSpeed = 0;
	            moveTracksMessage.rightTrackSpeed = 0;
	            m_victorUart.doMoveTracks(moveTracksMessage);
	            m_victorUart.doMoveTracks(moveTracksMessage);
	            m_victorUart.doMoveTracks(moveTracksMessage);
	            m_victorUart.doMoveTracks(moveTracksMessage);
	            m_victorUart.doMoveTracks(moveTracksMessage);
	            done = true;
	        	
	        }

        }
    }
    raspberryIf::MoveTracksMessage moveTracksMessage;
    moveTracksMessage.leftTrackSpeed = 0;
	moveTracksMessage.rightTrackSpeed = 0;
    m_victorUart.doMoveTracks(moveTracksMessage);
    m_victorUart.doMoveTracks(moveTracksMessage);
    return true;
}



void Approach::abort()
{

}
