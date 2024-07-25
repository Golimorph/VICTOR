#ifndef INVERSE_KINEMATICS_H
#define INVERSE_KINEMATICS_H

#include <vector>

#define A1  41.85 //OK
#define A2  44.79 //OK
#define B  129.9 //OK
#define C1  17.6 //OK
#define C2  122.45 //OK 
#define D1  6.41 //OK
#define D2  43.57 //OK
#define E1  39.51 //OK
#define E2  8.28 //OK
#define F1  19.2 //OK
#define F2  71.53 //OK

class InverseKinematics 
{
public:
    InverseKinematics();

    /*! @brief returns a set of angles "solution" given a desired coordinate and euler angles "desiredValue"
     *  The robot arm has its limitations and it is not always possible to move the arm to the desired location
     * for this case the method returns the last known solution, as this object stores the solution.
     * degrees is used for all units.
     *  @param desiredValue the x,y,z,alpha,beta,gamma. global coordinates and euler angle of the end effector. [Degrees].
     *  @param solution the set of angles a,b,c,d,e,f for the servos of the arm in order. f is the last servo before the end effector. [Degrees].
     *  @return true if it was possible to move to the desired location. If false the solution is the last solution.*/
    bool solve(const std::vector<double>& desiredValue, std::vector<double>& solution);

private:

	void compute_functions(const std::vector<double>& vars, std::vector<double>& funcs);
    void compute_jacobian(const std::vector<double>& vars, std::vector<std::vector<double> >& J);
    std::vector<double> solve_system(const std::vector<std::vector<double> >& J, const std::vector<double>& funcs);
    /*! @brief normalize the solution to within -M_PI to M_PI */
    void normalize(std::vector<double> &solution);
    /*! @brief check if the presented solution is within the abilities of the robot arm 
     * @paramm solution the solution to check
     * @return true if the solution is possible for the robot arm.*/
    bool isValid(const std::vector<double>& solution);

    /*! @brief Internally all angles are stored in radians. But externally this class uses degrees.
     *  @param varsRadians servo angles vector (a,b,c,d,e,f) in radians. */
    void convertToDegrees(std::vector<double>& varsRadians);

    /*! @brief Internally all angles are stored in radians. But externally this class uses degrees.
     *  @param desiredValues x,y,z,alpha,beta,gamma where x,y,z is in mm and alpha,beta,gamma is in radians. 
     *  @return last three parameters, alpha,beta,gamma will be in radians.*/
    std::vector<double> convertDesiredValuesToRadians(std::vector<double> desiredValues);

    /*! @brief check if the presented solution is within the abilities of the robot arm 
     * @paramm solution the solution to check
     * @return true if the solution is possible for the robot arm.*/
    bool solveForGuess(std::vector<double>& solution, const std::vector<double> guess);
    

    std::vector<double> m_desiredValue; //x,y,z,alpha,beta,gamma
    std::vector<double> m_lastSolution; //a,b,c,d,e,f angles of serovs
    std::vector<double> m_limits; //min_a,max_a,...,min_f,max_f
    std::vector<std::vector<double> > knownSolutions; //solution0,...,solution15, solutions on the format (a,b,c,d,e,f) angles of servos.

    inline static const double epsilon{1e-4};
    inline static const int max_iterations{2000};

};


#endif // INVERSE_KINEMATICS_H
