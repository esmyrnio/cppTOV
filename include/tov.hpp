#ifndef TOV_HPP
#define TOV_HPP
#include "eos.hpp"
#include <vector>
using Vector = std::vector<double>;
using Array = double*;
using String = std::string;
//this is the main class where the TOV equations are defined and solved
class TOV{
    public:
        TOV(String,double); //tabulated EoS constructor
        TOV(double,double,double); //polytropic EoS constructor
        Vector linspace(double, double, int); //linear space fector
        Vector gradient(Vector, double); //find gradient of vector
        void tovEquations(double, Array, Array); // TOV equations
        void integrateTOV(); //integration routine
        void printTabModel();
        void printPolyModel();
        void printSolutionProfile();
    private:
        double rtol,atol; //LSODA relative and absolute tolerance
        double eps; // energy density
        double eps_c; // central energy density
        double r,rout,dr; // LSODA integration points and grid's step size
        double surfaceIndex; // grid index of surface
        double m_c,nu_c,p_c; // starting solution vector's values
        double m_1,nu_1,p_1; // first solution vector's values via Taylor's expansion
        Vector r_vec; //distance vector
        Vector y,yout,dnudr; // solution vectors
        int idx,idxlast,neq,istate; // index and LSODA parameters
        EOS eos; // EoS class instance
        String eos_name; // Tabulated EoS file name
        double kappa,gamma; // polytropic EoS parameters
    public:
        double mass,radius;
        Vector radiusProfile,massProfile,metricProfile,pressureProfile,densityProfile; // solution vectors
};
#endif