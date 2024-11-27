#include "approach.h"

Approach::Approach(VictorUart& victorUart, Camera& camera, std::string object)
:m_victorUart(victorUart), m_camera(camera), m_object(object)
{
}


bool Approach::execute()
{
	if(m_camera.getDetections(m_object))
	{
		raspberryIf::MoveTracksMessage moveTracksMessage;
	    moveTracksMessage.leftTrackSpeed = 50;
	    moveTracksMessage.rightTrackSpeed = 50;
		m_victorUart.doMoveTracks(moveTracksMessage);
	}
	return true;
}



void Approach::abort()
{

}
