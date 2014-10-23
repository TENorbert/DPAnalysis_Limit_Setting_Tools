/* fscanf example */
#include <stdio.h>
#include <iostream>

using namespace std;
int main (int argc,char* argv[])
//int main ()
{
  char str [80];
  float time;
  float volt;
  float current;
  int read;
  FILE * pFile;
  std::string filename = argv[1];
  //std::string filename;
  //pFile = fopen ("myfile.txt","w+");
  pFile = fopen(filename.c_str(),"r+");
  //fprintf (pFile, "%f %s", 3.1416, "PI");
  //rewind (pFile);
  read = fscanf (pFile, "%f", &time);
  read = fscanf (pFile, "%f", &volt);
  read = fscanf (pFile, "%f", &current);
  //read = fscanf (pFile, "%s", str);
  fclose (pFile);
  printf ("I have read: %f, %f and %f \n",time, volt, current);
 if( read != 1 ) cout<<" Error Reading file ... " << endl; exit(0);
  return (0);
}
