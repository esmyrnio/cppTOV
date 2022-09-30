#include "../include/eos.hpp"
#include "../include/constants.hpp"
#include "../boost/boost/bind.hpp"
#include <cmath>
#include <functional>

using namespace SURFACE;
// using namespace UNITS;
using namespace CGS;
using boost::math::interpolators::pchip; //Piecewise Cubic Hermite interpolation
using boost::math::tools::brent_find_minima; //Brent's method to locate minima of function 

EOS::EOS(std::string eos_file):isPoly(false){
  std::string eos;
  eos="EoS/"; //set path
  eos+=eos_file;
  // check to see if file is valid
  if((f_eos=fopen(eos.c_str(),"r")) == NULL ) {    
      std::cout<<"cannot open file "<<eos_file<<std::endl; 
      exit(0);
  }
  //load EoS file parameters in log vectors
  fscanf(f_eos,"%d\n",&n_tab);
  for(i=1;i<=n_tab;i++) {  
      fscanf(f_eos,"%lf %lf %lf %lf\n",&rho,&p,&h,&n0) ; 
      log_e_tab.push_back(log10(rho/Density));
      log_p_tab.push_back(log10(p/(Density*pow(C,2))));
      log_h_tab.push_back(log10(h/(pow(C,2))));
      log_n0_tab.push_back(log10(n0));
      log_rho_tab.push_back(log10(MB*n0*pow(Length,3)/MSUN));  
    }
  p_surface=pow(10,log_p_tab[0]);
  fclose(f_eos);
}
EOS::EOS(double Kappa, double Gamma,double eps_c):Gamma(Gamma),Kappa(Kappa),eps_c(eps_c*pow(10,15)/CGS::Density),isPoly(true),p_surface(0.0)
{}
double EOS::e_at_p(double pp)
{
  // pchip interpolation of tabulated EoS points
  if(!isPoly){
    auto spline = pchip<decltype(log_p_tab)>(log_p_tab,log_e_tab);
    double eTab = pp<p_surface? 0 : pow(10.0,spline(log10(pp)));
    return eTab;
  }
  else{
    double ePoly = pow(pp/Kappa,1.0/Gamma) + pp/(Gamma-1);
    return ePoly;
  }
}
double EOS::pressureMinima(double pressure){return fabs(pressure-Kappa*pow(this->eps_c-pressure/(Gamma-1.0),Gamma));}
double EOS::p_at_e(double ee)
{
  // pchip interpolation of tabulated EoS points
  if(!isPoly){
    auto spline = pchip<decltype(log_e_tab)>(log_e_tab,log_p_tab);
    double pTab = pow(10.0,spline(log10(ee)));
    return pTab;
  }
  else{
    /*this function is called only to find the central pressure given the value of central energy density  
    thus we must use root-finding to find the actual value since central energy density epsilon is different from central density rho*/
    double p_c_approx = Kappa*pow(this->eps_c,Gamma); //set approximate value for central pressure
    const int double_bits = std::numeric_limits<double>::digits; // set numerical accuracy of double type for Brent's method
    /* using Brent's method alone is not enough to locate the desired minima, since the function pressureMinima has in fact two roots.
    For that reason we must find the first root of brent's method and to do that we check whether the function at the root obtained 
    is decreasing, in which case we lower the upper bound (upperCoef) of the method's bracketing to gradually reach the first root*/
    double upperCoef = 15.0; 
    double lowerCoef = 0.1;
    p_c_poly = brent_find_minima(boost::bind(&EOS::pressureMinima,this,_1), lowerCoef*p_c_approx,upperCoef*p_c_approx,double_bits);
    double pressureCheck = p_c_poly.first+0.005;
    bool BrentWentBad = (pressureCheck-Kappa*pow(this->eps_c-pressureCheck/(Gamma-1.0),Gamma))<0 || fabs(p_c_poly.second) || std::isnan(p_c_poly.second);
    while(BrentWentBad && upperCoef>=0.1){
      upperCoef-=0.01;
      p_c_poly = brent_find_minima(boost::bind(&EOS::pressureMinima,this,_1), lowerCoef*p_c_approx,upperCoef*p_c_approx,double_bits);
      pressureCheck = p_c_poly.first+0.005;
      BrentWentBad = (pressureCheck-Kappa*pow(this->eps_c-pressureCheck/(Gamma-1.0),Gamma))<0 || fabs(p_c_poly.second)>1e-5 || std::isnan(p_c_poly.second);
    }
    return p_c_poly.first;
  }
}

