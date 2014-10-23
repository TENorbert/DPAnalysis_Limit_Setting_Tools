#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

int main(){

    string line, cell;
    //ifstream myfile("example.txt");
    //ifstream myfile("sample.dat");
    ifstream myfile("limitfile.log");
    // try to open the file, else if you cannot throw exception
    try 
    {
        myfile.is_open();

            // read each line into line
            while (getline(myfile, line))
            {
                cout << "Line contains: " << line << endl;
                // copy the line into lineSteam, so we can store them into a string delimited 
                stringstream  lineStream(line);
                while (getline(lineStream, cell, ' '))
                {
                    cout <<"Words delimited by space: "<< cell << endl;
                }
            }
    }
    // display the exception thrown
    catch (ifstream::failure e) {
        cout << "Exception opening/reading file: " << e.what();
    }

    return 0;
}
