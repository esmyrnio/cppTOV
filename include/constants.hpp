#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
//centimetre–gram–second system of units
namespace CGS{
    extern const double G; //Newton's gravitational constant in cgs
    extern const double C; //speed of light in cgs
    extern const double MSUN; //solar mass in cgs
    extern const double Length; //length dimension in cgs
    extern const double Time; //time dimension in cgs
    extern const double Density; //density dimension in cgs
    extern const double MB; //baryon number density in cgs
};
//constants used in LSODA integration
namespace INTEGRATION{
    extern const double r_max_surface; //maximum surface radius in dimensionless units
    extern const int RDIV; //total number of grid points
};
//EoS related parameters at surface
namespace SURFACE{
    extern double p_surface; //pressure at surface
    extern double e_surface; //energy density at surface
    extern const double enthalpy_min; //enthalpy at surface
};
//
// namespace EXECUTION{
//     extern char* call_type;
//     extern bool CALL_MAIN;
// }
// namespace UNITS{
//     extern const double KAPPA;
//     extern const double KSCALE;
// };
#endif