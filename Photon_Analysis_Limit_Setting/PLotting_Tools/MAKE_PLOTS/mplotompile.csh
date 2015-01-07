#!/bin/tcsh
g++ PLot_Limit.cc -o doplots `root-config --cflags --libs` 
#g++ -c `root−config --cflags --glibs` FitHist.cc
#g++ -o HFit `root-config --glibs` FitHist.o
