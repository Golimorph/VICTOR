
#pragma once

#include <chrono>
#include <thread>
#include <wiringPi.h>
#include <map>
#include <iostream>
#include <vector>

/*! @brief class for controlling the lights on victor.*/
class Lights
{
public:

	Lights();

	enum class LightType 
	{
		HEADLIGHTS,//both front LED lights on victor
		LEFT_HEADLIGHT, //left front LED on victor
		RIGHT_HEADLIGHT //left front LED on victor
	};

	void on(LightType light);
	void off(LightType light);
	void blink(LightType light, std::chrono::milliseconds duration);
	void pulse(LightType light, std::chrono::milliseconds duration);

private:
	//maps the light to the corrsponding GPIO pin, pin numbering according
	//to wiringPi, which is not the same as physical pin, a lightType can map to serveral pins.
	std::map<LightType,std::vector<uint8_t>> m_lightPinMap;


	void onInternal(LightType light);
	void offInternal(LightType light);

	void blockingBlink(LightType light, std::chrono::milliseconds duration);
	std::thread m_workerThread;
	bool m_doWork = false;
};






