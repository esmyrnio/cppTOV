#include <iostream>
#include <stdio.h>
#include "../include/constants.hpp"
#include "../include/eos.hpp"
#include "../include/tov.hpp"
#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;


class Model{

  public:
  Model(String eos_name, double eps_c):
    model(eos_name,eps_c)
    {
      model.integrateTOV();
      mass = model.mass;
      radius = model.radius;
    };
  Model(double Kappa, double Gamma, double eps_c):
    model(Kappa,Gamma,eps_c)
    {
      model.integrateTOV();
      mass = model.mass;
      radius = model.radius;
    };
  private:
    TOV model;

  public: 
    double mass,radius;

    Vector returnMassProfile(){
      Vector massProfile = model.massProfile;
      return massProfile;
    }
    Vector returnRadiusProfile(){
      Vector radiusProfile = model.radiusProfile;
      return radiusProfile;
    }
    Vector returnMetricProfile(){
      Vector metricProfile = model.metricProfile;
      return metricProfile;
    }
    Vector returnPressureProfile(){
      Vector pressureProfile = model.pressureProfile;
      return pressureProfile;
    }
    Vector returnDensityProfile(){
      Vector densityProfile = model.densityProfile;
      return densityProfile;
    }

};

EMSCRIPTEN_BINDINGS(Model)
{
  class_<Model>("Model")
    .constructor<String,double>()
    .constructor<double,double,double>()
    .function("returnMassProfile",&Model::returnMassProfile)
    .function("returnRadiusProfile",&Model::returnRadiusProfile)
    .function("returnMetricProfile",&Model::returnMetricProfile)
    .function("returnPressureProfile",&Model::returnPressureProfile)
    .function("returnDensityProfile",&Model::returnDensityProfile)
    .property("mass",&Model::mass)
    .property("radius",&Model::radius)
    ;
  register_vector<double>("vector<double>");
    
}

