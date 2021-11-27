/****************************************************************************
  FileName     [ p2Main.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define main() function ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include <sstream>
#include "p2Table.h"
using namespace std;

int main()
{
   Table table;

   // Read in the csv file. Do NOT change this part of code.
   string csvFile;
   cout << "Please enter the file name: ";
   cin >> csvFile;
   if (table.read(csvFile))
     {
      cout << "File \"" << csvFile << "\" was read in successfully." << endl;
      table.assigntable(csvFile);
     }
   else {
      cerr << "Failed to read in file \"" << csvFile << "\"!" << endl;
      exit(-1); // csvFile does not exist.
   }
   // TODO read and execute commands
   string command;
   while (getline(cin,command)) {

      if(command[0] == 'P' && command[1] == 'R' && command[2] == 'I') table.print();  


      else if(command[0] == 'S' && command[1] == 'U' && command[2] == 'M') {
         command.erase(0,4);
         stringstream stream(command);
         int col = 0;
         stream>>col;
         table.sum(col);
      }


      else if(command[0] == 'A' && command[1] == 'V' && command[2] == 'E') {
         command.erase(0,4);
         stringstream stream(command);
         int col = 0;
         stream>>col;
         table.ave(col);
      }


      else if(command[0] == 'M' && command[1] == 'A' && command[2] == 'X') {
         command.erase(0,4);
         stringstream stream(command);
         int col = 0;
         stream>>col;
         table.max(col);
      }


      else if(command[0] == 'M' && command[1] == 'I' && command[2] == 'N') {
         command.erase(0,4);
         stringstream stream(command);
         int col = 0;
         stream>>col;
         table.min(col);
      }


      else if(command[0] == 'D' && command[1] == 'I' && command[2] == 'S') {
         command.erase(0,4);
         stringstream stream(command);
         int col = 0;
         stream>>col;
         table.dist(col);
      }


      else if(command[0] == 'A' && command[1] == 'D' && command[2] == 'D') {
         command.erase(0,4);
         table.add(command);
      }


      else if(command[0] == 'E' && command[1] == 'X' && command[2] == 'I') table.EXIT();
   }
}
