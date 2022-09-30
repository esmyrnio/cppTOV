#include <iostream>
#include <stdio.h>
#include "../include/constants.hpp"
#include "../include/eos.hpp"
#include "../include/tov.hpp"

char eos_name[80]; //tabulated EoS file
double central_density; //central density input
double Kappa,Gamma; //polytropic parameters input
char MODE; //mode for tabulated or polytropic EoS
int PRINT; //print option

int main(int argc, char *argv[]){
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
    //tabulated case
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
    //polytropic case
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