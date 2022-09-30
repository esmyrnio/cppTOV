# cppTOV

This is the TOV solver used in [quickStar](https://github.com/esmyrnio/quickStar), for [tabulated](https://ui.adsabs.harvard.edu/abs/2021PhRvD.103l3004B/abstract)[^1] ```(EoS/)``` or polytropic equations of state. The bindings used to expose the ```TOV``` class to JavaScript are located in ```src/bindings.cpp```.
The Tolman–Oppenheimer–Volkoff (TOV) system of ODEs is solved by implementing a [C++11 version of LSODA](https://github.com/dilawar/libsoda-cxx)[^2], which has been slightly modified to fit the ```TOV``` class. 
Additionaly, some modules from the [BOOST](https://github.com/boostorg/boost)[^3] C++ library are used to interpolate the tabulated equations of state, 
and also to find the central pressure in polytropic equations of state.

## Usage

Compile, using the provided makefile.

The executable inputs are specified using the following flags:

- **-m** *EoS model* ('p' for polytrope, 't' for tabulated)
- **-f** *eos name* (EoS file name, if EoS is tabulated).
- **-g** *Gamma* (polytropic parameter, if EoS is polytropic)
- **-k** *Kappa* (polytropic parameter, if EoS is polytropic)
- **-e** *central energy density* (central energy density in CGS/10^15).
- **-p** *print_option*:
    -  0: Prints gravitational mass M and radius R.
    -  1: Prints (0) along with the distance, metric, energy density and pressure profiles.

For example,

```
./TOV -m t -f ppsly4.cold.rns1.1.txt -e 2.5
```
```
./TOV -m p -g 2.0 -k 100.0 -e 2.5
```


[^1]:https://ui.adsabs.harvard.edu/abs/2021PhRvD.103l3004B/abstract (table IX)
[^2]:https://github.com/dilawar/libsoda-cxx
[^3]:https://github.com/boostorg/boost

