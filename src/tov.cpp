#include "../include/constants.hpp"
#include "../include/tov.hpp"
#include "../include/LSODA.hpp"
using boost::math::interpolators::pchip;
using namespace INTEGRATION;
TOV::TOV(String eos_name,double eps_c):eos(eos_name),eos_name(eos_name),eps_c(eps_c*pow(10,15)/CGS::Density),neq(3),rtol(1e-06),atol(1e-26)
{}
TOV::TOV(double Kappa, double Gamma, double eps_c):eos(Kappa,Gamma,eps_c),kappa(Kappa),gamma(Gamma),eps_c(eps_c*pow(10,15)/CGS::Density),neq(3),rtol(1e-12),atol(1e-50)
{}
// returns a linear space grid starting from "star" to "end" with "num" grid points 
std::vector<double> TOV::linspace(double start, double end, int num)
{
  std::vector<double> linspaced;
  if (num == 0) { return linspaced; }
  if (num == 1) 
    {
      linspaced.push_back(start);
      return linspaced;
    }
  double delta = (end - start) / (num - 1);
  for(int i=0; i < num-1; ++i)
    {
      linspaced.push_back(start + delta * i);
    }
  linspaced.push_back(end); 
  return linspaced;
}
// returns the gradient of a vector
std::vector<double> TOV::gradient(std::vector<double> input, double h)
{
    if (input.size() <= 1) return input;
    std::vector<double> res;
    for(int j=0; j<input.size(); j++) {
        int j_left = j - 1;
        int j_right = j + 1;
        if (j_left < 0) {
            j_left = 0;
            j_right = 1;
        }
        if (j_right >= input.size()){
            j_right = input.size() - 1; 
            j_left = j_right - 1;
        }
        double dist_grad = (input[j_right] - input[j_left]) / (2.0*h);
        res.push_back(dist_grad);
    }
    return res;
}
// define TOV equations
void TOV::tovEquations(double r, Array y, Array yprime)
{
    eps = y[2]<eos.p_surface? 0.0:eos.e_at_p(y[2]); // checks if surface is reached
    yprime[0] = 4*M_PI*pow(r,2.0)*eps;
    yprime[1] = 2.0*(y[0] + 4.0*M_PI*pow(r,3.0)*y[2])/(r*(r-2.0*y[0]));
    yprime[2] = -(eps + y[2])*(y[0] + 4.0*M_PI*pow(r,3.0)*y[2])/(r*(r-2.0*y[0]));
}
// main integration routine
void TOV::integrateTOV()
{   
    istate=1;
    r_vec = linspace(0.0,r_max_surface,RDIV);
    dr=r_vec[1]-r_vec[0];
    m_c = 0.0;
    nu_c = -1.0;
    p_c = eos.p_at_e(eps_c);
    m_1 =(4.0/3.0)*M_PI*eps_c*pow(dr,3.0);
    nu_1= nu_c + 4.0*M_PI*(p_c+(1.0/3.0)*eps_c)*pow(dr,2.0);
    p_1= p_c - (2.0*M_PI)*(eps_c+p_c)*(p_c+(1.0/3.0)*eps_c)*pow(dr,2.0);
    y = {m_c,nu_c,p_c};
    massProfile.push_back(y[0]);
    metricProfile.push_back(y[1]);
    pressureProfile.push_back(y[2]);
    densityProfile.push_back(eos.e_at_p(y[2]));
    radiusProfile.push_back(0.0);
    r=dr;
    rout=r+dr;
    idx=1;
    y = {m_1,nu_1,p_1};
    LSODA lsoda; // call LSODA
    // integration stops when pressure drops below tabulated EoS last point or 0.0 in polytropic case
    while(y[2]>eos.p_surface && r<r_vec[RDIV-1]){
        radiusProfile.push_back(r);
        massProfile.push_back(y[0]);
        metricProfile.push_back(y[1]);
        pressureProfile.push_back(y[2]);
        densityProfile.push_back(eos.e_at_p(y[2]));
        lsoda.lsoda_update(this, &TOV::tovEquations, neq, y, yout, &r, rout, &istate,rtol,atol);
        rout += dr;
        y[0] = yout[1];
        y[1] = yout[2];
        y[2] = yout[3];
        idx += 1;
        if(istate==-3 || isnan(massProfile[idx-2]) || isnan(pressureProfile[idx-2])) break;
  }
  idxlast=idx-1;
  surfaceIndex = idxlast;
  radius = r_vec[idxlast]*CGS::Length/pow(10,5);
  mass = massProfile[idxlast];
  double metric_surface_old = metricProfile[idxlast];
  double metric_surface = log(1.0-2.0*mass/radius);
  // correct metric profile to match the analytic solution in the exterior
  for (auto& nu:metricProfile){
    nu+=metric_surface-metric_surface_old;
  } 
}
void TOV::printTabModel(){
  printf("\n");
  printf("  %s                  \n\n",eos_name.c_str());
  printf("  %2.3e     CENTRAL DENSITY         (10^15 gr/cm^3)\n",eps_c*CGS::Density/pow(10,15));
  printf("\n");
  printf("  %2.3e     MASS                    (M_sun)\n",mass);
  printf("  %2.3e     RADIUS                  (km)\n",radius);
  printf("\n");
}
void TOV::printPolyModel(){
  printf("\n");
  printf("  %2.3e     KAPPA           \n",kappa);
  printf("  %2.3e     GAMMA           \n",gamma);
  printf("  %2.3e     CENTRAL DENSITY         (10^15 gr/cm^3)\n",eps_c*CGS::Density/pow(10,15));
  printf("\n");
  printf("  %2.3e     MASS                    (M_sun)\n",mass);
  printf("  %2.3e     RADIUS                  (km)\n",radius);
  printf("\n");
}
void TOV::printSolutionProfile(){
    printf("---------------------------------------------------------\n");   
    printf("     r        m(r)        nu(r)     rho(r)     P(r)\n");
    printf("---------------------------------------------------------\n");   
    for(int i=0;i<surfaceIndex;i++){
      printf("%5.4e %5.4e %5.4e %5.4e %5.4e\n",
        radiusProfile[i], massProfile[i], metricProfile[i], densityProfile[i],pressureProfile[i]);
    }
}