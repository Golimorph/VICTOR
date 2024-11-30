#include "stateMachine.h"


StateMachine::StateMachine(VictorUart& victorUart, Camera& camera)
    :m_victorUart(victorUart), m_camera(camera)
{

}

void StateMachine::start()
{
    m_isRunning = true;
    m_programThread = std::thread(&StateMachine::program, this);
}

void StateMachine::stop()
{
    m_isRunning = false;
    m_programThread.join();
}

void StateMachine::changeState(State newState)
{
    m_state = newState;
}

void StateMachine::checkSensors()
{

}

void StateMachine::program()
{
    while (m_isRunning)
    {
        switch(m_state)
        {
        case State::EXPLORE:
            //Implement the exlore state
            break;
        case State::APPROACH:
            Approach approach(m_victorUart, m_camera, "mouse");
            approach.execute();
            break;
        }
    }
}














