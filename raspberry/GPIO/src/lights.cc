

#include <lights.h>


Lights::Lights()
{
	// Initialize wiringPi using its own numbering scheme
    if (wiringPiSetup() == -1) 
    {
        std::cerr << "wiringPi setup failed!" << std::endl;
    }


    m_lightPinMap = {
    	{LightType::HEADLIGHTS,{7,29}},
    	{LightType::LEFT_HEADLIGHT,{29}},
    	{LightType::RIGHT_HEADLIGHT,{7}}
    };
    pinMode(m_lightPinMap.at(LightType::LEFT_HEADLIGHT).at(0), OUTPUT);
    pinMode(m_lightPinMap.at(LightType::RIGHT_HEADLIGHT).at(0), OUTPUT);
}



void Lights::onInternal(LightType light)
{
	std::vector<uint8_t> pins = m_lightPinMap.at(light);
	for(uint8_t pin : pins)
	{
		digitalWrite(pin, HIGH);
	}
}

void Lights::on(LightType light)
{
	if(m_doWork)
	{
		m_doWork=false;
		m_workerThread.join();
	}
	onInternal(light);
}

void Lights::offInternal(LightType light)
{
	std::vector<uint8_t> pins = m_lightPinMap.at(light);
	for(uint8_t pin : pins)
	{
		digitalWrite(pin, LOW);
	}
}

void Lights::off(LightType light)
{
	if(m_doWork)
	{
		m_doWork=false;
		m_workerThread.join();
	}
	offInternal(light);
}

void Lights::blockingBlink(LightType light, std::chrono::milliseconds duration)
{
	while(m_doWork)
	{
		onInternal(light);
		std::this_thread::sleep_for(duration);
		offInternal(light);
		std::this_thread::sleep_for(duration);
	}
}

void Lights::blink(LightType light, std::chrono::milliseconds duration)
{
	m_doWork = true;
	m_workerThread = std::thread(&Lights::blockingBlink, this, light, duration);
}

void Lights::pulse(LightType light, std::chrono::milliseconds duration)
{

}



















