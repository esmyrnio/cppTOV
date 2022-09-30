#include <iostream>
#include <stdio.h>
#include "../include/constants.hpp"
#include "../include/eos.hpp"
#include "../include/tov.hpp"

char eos_name[80];
double central_density;
double Gamma;
double Kappa;
char MODE;
int PRINT;

// void get_MR(std::string _eos_name, double _central_density, double* mass, double* radius){
//     TOV model(_eos_name, _central_density);
//     model.integrateTOV();
//     *mass = model.mass;
//     *radius = model.radius;
// }

// void get_MR(double _Kappa, double _Gamma, double _central_density, double* mass, double* radius){
//     TOV model(_Kappa,_Gamma,_central_density);
//     model.integrateTOV();
//     *mass = model.mass;
//     *radius = model.radius;
// }



int main(int argc, char *argv[]){
    // EXECUTION::call_type = "main_call";
    // EXECUTION::CALL_MAIN = true;
    for(int i=1;i<argc;i++) 
      if(argv[i][0]=='-'){
        switch(argv[i][1]){
            case 'f':
                sscanf(argv[i+1],"%s",eos_name);
                break;
            case 'e':
                sscanf(argv[i+1],"%lf",&central_density);
                break;
            case 'g':
                sscanf(argv[i+1],"%lf",&Gamma);
                break;
            case 'k':
                sscanf(argv[i+1],"%lf",&Kappa);
                break;
            case 'm':
                sscanf(argv[i+1],"%c",&MODE);
                break;
            case 'p':
                sscanf(argv[i+1],"%i",&PRINT);
                break;

          }
      }
	

    switch (MODE)
    {
    case 't':{
        TOV tabModel(eos_name,central_density);
        tabModel.integrateTOV();
        switch (PRINT)
        {
        case 0:
            tabModel.printTabModel();
            break;
        case 1:
            tabModel.printTabModel();
            tabModel.printSolutionProfile();
            break;
        default:
            break;
        }
        break;
    }
    case 'p':{
        TOV polyModel(Kappa,Gamma,central_density);
        polyModel.integrateTOV();
        switch (PRINT)
        {
        case 0:
            polyModel.printPolyModel();
            break;
        case 1:
            polyModel.printPolyModel();
            polyModel.printSolutionProfile();
            break;
        default:
            break;
        }
        break;
    }
    default: 
        exit(1);
        break;

    }

    return 0;
}