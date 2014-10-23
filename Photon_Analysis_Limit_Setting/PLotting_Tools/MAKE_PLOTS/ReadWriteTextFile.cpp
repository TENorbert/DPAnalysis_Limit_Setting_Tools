#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

//Define Line
struct Line {
  double time, volt, ampere;
  int linenumber;
};
// Use Read in operator
istream& operator>>(istream& in, Line& line) {
    ++line.linenumber;
    return in >> line.time >> line.volt >> line.ampere;
}
//Use read Out Operator
ostream& operator<<(ostream& out, const Line& line) {
  return out << "Line: " << line.linenumber 
             << " Time: " << line.time 
             << " Volt: " << line.volt 
             << " Ampere: " << line.ampere;
}

// Now do main
int main(int argc, char* argv[] )
{
    string dummy;
    vector<float> vecT;
    vector<float> vecV;
    vector<float> vecA;
    if(argc < 3) { cout <<" Sorry... give me an iput file..." << endl; exit(0);}
    //std::istream& in = cin; 
    std::ifstream in(argv[1],ios::in);
    in >> dummy >> dummy >> dummy;
    Line max;
    Line line;
    while(in.good()) {
        in >> line;
        vecT.push_back(line.time); vecV.push_back(line.volt); vecA.push_back(line.ampere);
        if (line.volt > max.volt) max = line;
    }
   //cout << "Max line: " << max;
   ofstream ofile;
   ofile.open(argv[2],ios::out);
  
   for(unsigned i = 0; i < vecT.size(); i++) 
     {
      if(i==0) ofile << "Time(s)\t\t\t" <<"Voltage(V)\t\t" << "Current(A)" << endl; 
      ofile << vecT[i] << "\t\t\t" << vecV[i] << "\t\t\t" << vecA[i] << endl;
      cout <<" Voltage := " << vecV[i] << "  Time: = " << vecT[i] << " Current: =  " << vecA[i] << endl;
     }
     ofile.close();
   return 0;
}
