/****************************************************************************
  FileName     [ p2Table.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Row and Table ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include "p2Table.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <iomanip>
#include <sstream>

using namespace std;

// Implement member functions of class Row and Table here
Row::Row(int size)
{
   _data = new int[size];
   for(int i = 0; i < size; i++)
      _data[i]=0;
}



bool
Table::read(const string& csvFile)
{
   fstream file(csvFile);
   if(file){
      
      return true;
   }
   else return false;
}



void Table::assigntable(const string& csvFile)
{
   fstream file(csvFile);
   string input1,input2;
   _nCols = 0;

   while(getline(file,input1)){
      int comma_count = 0;

      for(int i = 0; i < input1.size(); i++)
         if(input1[i] == ',') comma_count++;

      _nCols = comma_count+1;
      Row row(_nCols);
      istringstream filepart(input1);

      int counter = 0;
      while(getline(filepart,input2,',')){
        if(static_cast<int>(input2.back()) == 13) input2.pop_back();
        if(input2.size() == 0) row[counter] = INT_MAX;
        else{ stringstream number(input2); number>>row[counter]; }
        counter++;
      }
      _rows.push_back(row);
   }
}




void Table::print()
{
   for(int i = 0; i < _rows.size(); i++){
      for(int j = 0; j < _nCols; j++){
         if(_rows[i][j] == INT_MAX)cout<<setw(4)<<".";
         else cout<<setw(4)<<_rows[i][j];
      }
      cout<<endl;
   }
}

void Table::sum(int i)
{
   int sum = 0;
   for(int j = 0; j < _rows.size(); j++){
      if(_rows[j][i] == INT_MAX) continue;
      else sum =sum + _rows[j][i];
   }
   cout<<"The summation of data in column #"<<i<<" is "<<sum<<"."<<endl;
}

void Table::ave(int i)
{
   int sum = 0, count = 0;
   double ave;
   for(int j = 0; j < _rows.size(); j++){
      if(_rows[j][i] == INT_MAX) continue;
      else {sum =sum + _rows[j][i]; count++;}
   }
   ave = static_cast<double>(sum)/count;
   cout<<"The average of data in column #"<<i<<" is ";
   cout<<fixed<<setprecision(1)<<ave<<"."<<endl;
}

void Table::max(int i)
{
   int maxarray[_rows.size()];
   for(int j = 0; j < _rows.size(); j++){
      if(_rows[j][i] == INT_MAX) maxarray[j] = INT_MIN;
      else {maxarray[j] = _rows[j][i];}
   }
   int max = maxarray[0];
   for(int a = 1; a < _rows.size(); a++)
      if(maxarray[a] > max) max = maxarray[a];
   
   cout<<"The maximum of data in column #"<<i<<" is "<<max<<"."<<endl;
   
}

void Table::min(int i)
{
   int minarray[_rows.size()];
   for(int j = 0; j < _rows.size(); j++){
      if(_rows[j][i] == INT_MAX) minarray[j] = INT_MAX;
      else {minarray[j] = _rows[j][i];}
   }
   int min = minarray[0];
   for(int a = 1; a < _rows.size(); a++)
      if(minarray[a] < min) min = minarray[a];
   
   cout<<"The minimum of data in column #"<<i<<" is "<<min<<"."<<endl;   
}


void Table::dist(int i)
{
   int count = 0;
   for(int j = 0; j < _rows.size(); j++){
      if(_rows[j][i] == INT_MAX) continue;
      else count++;
   }

   int distarray[count];
   int j = 0;
   for(int a = 0; a < count; a++){
      for(j; j < _rows.size(); j++){
         if(_rows[j][i] == INT_MAX) continue;
         else {distarray[a] = _rows[j][i]; j++; break;}
      }
   }

   for(int x = 0; x < count; x++){
      for(int y = x+1; y <count; y++)
         if(distarray[x] == distarray[y]) distarray[y] = INT_MIN;
   }
   
   int dist = 0;
   for(int a = 0; a <count; a++){
      if(distarray[a] == INT_MIN) continue;
      else dist++;
   }
   cout<<"The distinct count of data in column #"<<i<<" is "<<dist<<"."<<endl;
}

void Table::add(const string& addstring)
{
   Row row(_nCols);
   string input;
   istringstream filepart(addstring);

   int counter = 0;
   while(getline(filepart,input,' ')){
     if(static_cast<int>(input.back()) == 13) input.pop_back();
     if(input[0] == '.') row[counter] = INT_MAX;
     else{ stringstream number(input); number>>row[counter]; }
     counter++;
   }
   _rows.push_back(row);
   
}

void Table::EXIT()
{
   exit(-1);
}



















































