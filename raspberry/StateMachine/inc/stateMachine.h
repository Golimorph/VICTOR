#pragma once
#include <sys/socket.h>
#include <thread>
#include "approach.h"
#include "camera.h"

/*!@brief The stateMachine class acts a central resource handler for victor. It continously runs and gathers
 * information from all available sensors on victor and changes victors top level behavior between a number
 * of descreet states. It runs in a separate thread and is always active.*/
class StateMachine 
{
public:
	StateMachine(VictorUart& victorUart, Camera& camera);

	/*! @brief Starts a new thread within which the stateMachine runs. The state machine will itself
	 *  make transitions between states depending on sensor inputs.*/
	void start();

	/*! @brief Stops the state machine. For debugging.*/
	void stop();

private:
	
	enum class State
	{
		EXPLORE,
		APPROACH,
		GRAB
	};
	State m_state = State::APPROACH;
	VictorUart& m_victorUart;
	Camera& m_camera;
	bool m_isRunning = false;
	std::thread m_programThread;


	void changeState(State newState);
	void checkSensors();
	void program();
};
