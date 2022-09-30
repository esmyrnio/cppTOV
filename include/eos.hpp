#ifndef EOS_HPP
#define EOS_HPP
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "../boost/math/interpolators/pchip.hpp"
#include "../boost/math/tools/minima.hpp"
// Equation of State class, covering tabulated and polytropic options
class EOS{
    public:
        EOS(std::string); //Tabulated EoS constructor
        EOS(double,double,double); //Polytropic EoS constructor
        EOS(); //default
    public:
        double p_surface,eps_c;
        double Gamma,Kappa; //polytropic EoS parameters
        bool isPoly;
        double p_at_e(double); //evaluate pressure at given energy density
        double e_at_p(double); //evaluate energy density at given pressure
        double pressureMinima(double); //minima function to evaluate central pressure via root-finding in polytropic EoS
    private:
        int i,n_tab; //number of tabulated points in tabulated EoS
        double p,rho,h,n0; //tabulated EoS file parameters
        FILE* f_eos; //tabulated EoS file
        std::vector<double> log_e_tab,log_p_tab,log_h_tab,log_n0_tab,log_rho_tab; //log tabulated EoS parameters
        std::pair<double,double> p_c_poly; //returned central pressure in polytropic EoS 
};
#endif