//created by Richard Edberg 2024
#ifndef INVERSE_KINEMATICS_H
#define INVERSE_KINEMATICS_H


#include <math.h>
#include <Arduino.h>


class InverseKinematics {
public:
    InverseKinematics();

    /*! @brief finds the angles a,b that corresponds to moving
     * the robot arm to coordinates r, z
     * 
     * @return false if the desired coordinates are not in mechanical range of the robot arm*/  
    bool solve(double &a, double &b, double &theta, double x, double y, double z);
   
    /*! @brief finds the angles omega and psi which give the direction of the claw
     * given that coordinates y and z are given for the direction of the robot arm
     * 
     * @return false if not mechanically possible to set the desired angle. */
    bool getClawAngle(double &omega, double &psi, double x, double y, double z);


private:
    bool newtonRaphsonSolver(double &a, double &b, double r, double z);
    //helper methods
    double normalizeAngle(double angle);
    bool isInRange(double a, double b);
    //The shape of the robot arm is given by a number of constants.
    double m_o, m_p, m_q, m_n, m_m;
    double START_GUESSES[6];
    double lastSolution[2];

    double m_a_limits[2];//min,max 
    double m_b_limits[2];//min,max
};

#endif // INVERSE_KINEMATICS_H
