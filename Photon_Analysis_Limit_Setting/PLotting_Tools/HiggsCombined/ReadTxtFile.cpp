#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

void SpitOut();
int main ()
{
  :w
 std::cout <<" Hello Crazy Boy!\n" << std::endl;
 SpitOut();

}

void SpitOut()
{
 std::cout <<" U Call me Crazy? U Mad?\n" << std::endl;

}

// Read Table
float** ReadTable(const char* FileName, int& RowNum, int& ColNum)
{
   string line;
   ifstream in_stream(FileName);
   ofstream out_stream;
 
   // Determine number of rows and columns in file
   // Program halts if rows are not of same length******************Doesn't Work*******************
    while(getline(in_stream,line,'\n')) 
    {
      string segments;
      int ColsPerRow = 0; // Initialize counter
      stringstream ss;
      ss << line;
      while (getline(ss,segments,'\t'))
      {
            ColsPerRow++;
       }
     
       if (RowNum == 0) 
       {
        // Define number of columns in file as the number
        // of columns in the first row.
          ColNum = ColsPerRow;
       } else {
               if (ColsPerRow != ColNum)
	         {
                  cerr << "Row " << RowNum << " is not the same length as row 0." << endl;
                  exit(0);
                 }
              }
     RowNum++;
    }
       
    // Declare arrays for storing and accessing
    // data from the file.
    float** pa2d = new float*[RowNum];
    float* parr = new float[RowNum*ColNum];
    // Reposition to start of stream buffer.
    in_stream.clear();
    in_stream.seekg(0, ios::beg);
    // Write data to array:
    for (int i = 0; i < (RowNum*ColNum); i++) 
    {
        // Declarations.
       float in_float;
       char in_char;
       string in_s;
       stringstream in_ss;
      // Insert data entry into stringstream.
       in_stream >> in_s;
       in_ss << in_s;
       // Convert data entry to float.
       in_ss >> in_float;
     }

     // Prepare 2D array to be returned.
     // Define pointer position at start of each row.
     for (int i = 0; i < RowNum; i++) 
     {
       pa2d[i] = parr + (i*ColNum);
      }
      
 return pa2d;
 
}

// Print Output matrix
void PrintMatrix(float** matrix, const int RowNum, const int ColNum) {
// Print "matrix" to console.
cout << '\n';
for (int i=0; i < RowNum; i++) {
for(int j=0; j < ColNum; j++) {
cout << (j?"\t":"") << matrix[i][j];
// Don't insert tab before first element of each row.
}
cout << '\n';
}
}

// Output here!
void OutPut(float** matrix, const int RowNum, const int ColNum) {
//Print matrix to text file
ofstream out_stream;
out_stream.open("outfile.dat"); // open (connect) output file stream
 
if (out_stream.fail())
{
cout << "Output file opening failed.\n";
cout << '\n';
}
for (int i=0; i < RowNum; i++) {
for(int j=0; j < ColNum; j++) {
 
out_stream << (j?"\t":"") << matrix[i][j];
// Don't insert tab before first element of each row.
}
out_stream << '\n';
}
}


// Run Program here!
int main() {
// Define input filename.
const char* FileName = "test3.txt";
// Initialize row and column counters.
int RowNum = 0;
int ColNum = 0;
// Perform reading of data file into 2D array.
float** data = ReadTable(FileName,RowNum,ColNum);
 
PrintMatrix(data,RowNum,ColNum);
OutPut(data,RowNum,ColNum);
 
system ("pause");
return(0);
}
