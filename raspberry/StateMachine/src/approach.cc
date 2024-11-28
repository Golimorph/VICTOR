#include "approach.h"

Approach::Approach(VictorUart& victorUart, Camera& camera, std::string object)
:m_victorUart(victorUart), m_camera(camera), m_object(object)
{
}



int8_t Approach::clampToInt8(int value) {
    if (value > 127) {
        return 127; // Maximum value for int8_t
    } else if (value < -127) {
        return -127; // Minimum value in your desired range
    }
    return static_cast<int8_t>(value); // Cast safely if within range
}

bool Approach::execute()
{
	while(true)
	{
		auto detection = m_camera.getDetection(m_object);
		if(detection.has_value())
		{
			int compValue = static_cast<int>(300*((detection.value().xmin + detection.value().xmax)/2 - 0.5));
			raspberryIf::MoveTracksMessage moveTracksMessage;
			moveTracksMessage.leftTrackSpeed = 50 + compValue;
			moveTracksMessage.rightTrackSpeed = 50 - compValue;	
			m_victorUart.doMoveTracks(moveTracksMessage);
		}
	}
	return true;
}



void Approach::abort()
{

}
