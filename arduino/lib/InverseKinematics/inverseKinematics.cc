#include "inverseKinematics.h"

InverseKinematics::InverseKinematics()
{
    //a,b,a,b,a,b three guesses to get a good start.
    lastSolution[0] = 0;//This will beupdated when a solution is found.
    lastSolution[1] = 0;//This will be updated when a solution is found.
    START_GUESSES[0] = 0;
    START_GUESSES[1] = 0;
    START_GUESSES[2] = 50;
    START_GUESSES[3] = 30;
    START_GUESSES[4] = -60;
    START_GUESSES[5] = 40;

    //The shape of the robot arm, size of each link in mm.
    m_o = 123.0+120;//remove this 130 if you want the distance to the wrist instead of the claw.
    m_p = 40.86;
    m_q = 44.83;
    m_n = 17.5;
    m_m = 131.34;
    //min and max values of the angles (somewhat outside real range, but just to be in the ballpark for the solver):
    m_a_limits[0] = -80;//min degrees
    m_a_limits[1] = 110;//max degrees
    m_b_limits[0] = -40;//min degrees
    m_b_limits[1] = 80;//max degrees
}

bool InverseKinematics::getClawAngle(double &omega, double &psi, double x, double y, double z)
{
    double apb = (lastSolution[0]+lastSolution[1])*M_PI/180;

    omega = acos(cos(apb)*y-sin(apb)*z);
    psi = asin(-(sin(apb)*y+cos(apb)*z)/sin(omega));


    omega *= 180/M_PI;
    psi *= 180/M_PI;
    return true;
}


bool InverseKinematics::solve(double &a, double &b, double &theta, double x, double y, double z)
{
    //horizontal radial distance to target.
    double r = sqrt(x*x+y*y);
    //The horizontal angle to the target (in the x,y plane).
    theta = asin(x/r)*180/M_PI;//degrees

    //For the vertical angles a,b in the robot arm, we need to find it by numerical solution.
    //use ad-hoc method with a number of different start guesses for the newton-raphson method.
    //reason for this is that there is no single guess that always converges.
    a = lastSolution[0];
    b = lastSolution[1];
    if(newtonRaphsonSolver(a,b, r, z))
    {
        lastSolution[0] = a;
        lastSolution[1] = b;
        return true;
    }
    for(int guess = 0; guess < (sizeof(START_GUESSES)/sizeof(START_GUESSES[0]))/2; ++guess)
    {
        a = START_GUESSES[2*guess];
        b = START_GUESSES[2*guess + 1];
        if(newtonRaphsonSolver(a,b, r, z))
        {
            lastSolution[0] = a;
            lastSolution[1] = b;
            return true;
        }
    }
    //Serial.println("Failed to solve InverseKinematics!, "+String(a)+", "+String(b));
    return false;
}

//help method for newtonRaphsonSolver
double InverseKinematics::normalizeAngle(double angle)
{
    while (angle > M_PI) angle -= 2 * M_PI;
    while (angle < -M_PI) angle += 2 * M_PI;
    return angle;
}

//help method for newtonRaphsonSolver
bool InverseKinematics::isInRange(double a, double b)
{
    return a > m_a_limits[0] && a < m_a_limits[1] && b > m_b_limits[0] && b < m_b_limits[1];
}

//returns true if a solution is found, solution in the a,b reference arguments. Radians.
bool InverseKinematics::newtonRaphsonSolver(double &a, double &b, double r, double z)
{
    const double TOLERANCE = 1e-3;//I dont need better tolerance than this, the servos are not so precise anyway.
    const int MAX_ITERATIONS = 30;

    //The sin/cos/solver work with radians need to convert.
    a = a*M_PI/180;
    b = b*M_PI/180;

    for (int iter = 0; iter < MAX_ITERATIONS; iter++)
    {
        double f1 = m_o * cos(a + b) + m_n * sin(a + b) + m_m * sin(a) + m_p - r;
        double f2 = -m_o * sin(a + b) + m_n * cos(a + b) + m_m * cos(a) + m_q - z;

        double J11 = -m_o * sin(a + b) + m_n * cos(a + b) + m_m * cos(a);
        double J12 = -m_o * sin(a + b) + m_n * cos(a + b);
        double J21 = -m_o * cos(a + b) - m_n * sin(a + b) - m_m * sin(a);
        double J22 = -m_o * cos(a + b) - m_n * sin(a + b);

        double det = J11 * J22 - J12 * J21;

        if (fabs(det) < 1e-10)
        {
            // The Jacobian determinant is too small, likely a singular matrix
            //Serial.println("Jacobian determinant is too small, stopping.");
            return false;
        }

        double delta_a = (J22 * f1 - J12 * f2) / det;
        double delta_b = (-J21 * f1 + J11 * f2) / det;

        a -= delta_a;
        b -= delta_b;

        if (fabs(delta_a) < TOLERANCE && fabs(delta_b) < TOLERANCE)
        {
            // Converged to a solution
            a = normalizeAngle(a)*180/M_PI; //convert back to degrees between -180 and 180.
            b = normalizeAngle(b)*180/M_PI;
            //Some solution are not within physical range, only return true if physically possible.
            //(i.e. arm cannot move to -150 for example)
            return isInRange(a,b);
        }
    }
    return false;
}



