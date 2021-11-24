/****************************************************************************
  FileName     [ dbTable.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Table member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <string>
#include <cctype>
#include <cassert>
#include <set>
#include <algorithm>
#include "dbTable.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream& operator << (ostream& os, const DBRow& r)
{
   // TODO: to print out a row.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
   for(int i=0;i<r.size();i++)
   {
     if(r._data[i]==INT_MAX) cout<<".";
     else cout<<r._data[i];
     if(i != (r.size()-1)) cout<<" ";
   }
   return os;
}

ostream& operator << (ostream& os, const DBTable& t)
{
   // TODO: to print out a table
   // - Data are seperated by setw(6) and aligned right.
   // - Null cells are printed as '.'
   for(int i = 0; i < t.nRows(); i++)
   {
     for(int j = 0; j < t.nCols(); j++)
     {
       if(t._table[i][j] == INT_MAX) cout<<setw(6)<<right<<".";
       else cout<<setw(6)<<right<<t._table[i][j];
     }
     cout<<endl;
   }
   return os;
}

ifstream& operator >> (ifstream& ifs, DBTable& t)
{
   // TODO: to read in data from csv file and store them in a table
   // - You can assume the input file is with correct csv file format
   // - NO NEED to handle error file format

   string input;
   int num;
   while(getline(ifs,input))
   {
     vector<int> RawData;
     int comma_count = 0;
     if(static_cast<int>(input.back()) == 13) input.pop_back();
     input.push_back(',');

     for(int i = 0; i < input.size(); i++)
        if(input[i] == ',') comma_count++;

     while(1)
     {
      int pos = 0;
      int comma = input.find_first_of(',',pos);
      if(comma == 0)
      {
        RawData.push_back(INT_MAX);
      }
      else
      {
        string temp = input.substr(0,comma);
        myStr2Int(temp,num);
        RawData.push_back(num);
      }
      pos =comma+1;
      input = input.substr(pos);
      if(RawData.size() == comma_count)
      {
        DBRow r(RawData);
        t.addRow(r);
        break;
      }
     }


   }
   return ifs;
}

/*****************************************/
/*   Member Functions for class DBRow    */
/*****************************************/
void
DBRow::removeCell(size_t c)
{
   // TODO
   _data.erase(_data.begin()+c);
}

/*****************************************/
/*   Member Functions for struct DBSort  */
/*****************************************/
bool
DBSort::operator() (const DBRow& r1, const DBRow& r2) const
{
   // TODO: called as a functional object that compares the data in r1 and r2
   //       based on the order defined in _sortOrder
   for(int i = 0; i < _sortOrder.size(); i++)
   {
     if(r1[_sortOrder[i]] != r2[_sortOrder[i]])
        return (r1[_sortOrder[i]] < r2[_sortOrder[i]]);
   }
   return false;
}

/*****************************************/
/*   Member Functions for class DBTable  */
/*****************************************/
void
DBTable::reset()
{
   // TODO
   _table.clear();
}

void
DBTable::addCol(const vector<int>& d)
{
   // TODO: add a column to the right of the table. Data are in 'd'.
   for(int i = 0; i < _table.size(); i++)
   {
     _table[i].addData(d[i]);
   }
}

void
DBTable::delRow(int c)
{
   // TODO: delete row #c. Note #0 is the first row.
   _table.erase(_table.begin()+c);
}

void
DBTable::delCol(int c)
{
   // delete col #c. Note #0 is the first row.
   for (size_t i = 0, n = _table.size(); i < n; ++i)
      _table[i].removeCell(c);
}

// For the following getXXX() functions...  (except for getCount())
// - Ignore null cells
// - If all the cells in column #c are null, return NAN
// - Return "float" because NAN is a float.
float
DBTable::getMax(size_t c) const
{
   // TODO: get the max data in column #c
   bool emptyCol = false;
   int maxarray[_table.size()];
   for(int i = 0; i < _table.size(); i++){
      if(_table[i][c] == INT_MAX) maxarray[i] = INT_MIN;
      else {maxarray[i] = _table[i][c]; emptyCol = true;}
   }
   if(emptyCol)
   {
     float max = maxarray[0];
     for(int i = 1; i < _table.size(); i++)
       if(maxarray[i] > max) max = maxarray[i];
    return max;   
   }
   else return NAN;
}

float
DBTable::getMin(size_t c) const
{
   // TODO: get the min data in column #c
   bool emptyCol = false;
   int minarray[_table.size()];
   for(int i = 0; i < _table.size(); i++){
      if(_table[i][c] == INT_MAX) minarray[i] = INT_MAX;
      else {minarray[i] = _table[i][c]; emptyCol = true;}
   }
   if(emptyCol)
   {
     float min = minarray[0];
     for(int i = 1; i < _table.size(); i++)
       if(minarray[i] < min) min = minarray[i];
    return min;   
   }
   else return NAN;
}

float 
DBTable::getSum(size_t c) const
{
   // TODO: compute the sum of data in column #c
   bool emptyCol = false;
   float sum=0;
   for(int i = 0; i < _table.size(); i++)
   {
     if(_table[i][c] != INT_MAX)
     {
       sum = sum + _table[i][c]; emptyCol = true;
     }

   }
   if(emptyCol) return sum;
   else return NAN;
}

int
DBTable::getCount(size_t c) const
{
   // TODO: compute the number of distinct data in column #c
   // - Ignore null cells
   int count = 0;
   for(int i = 0; i < _table.size(); i++){
      if(_table[i][c] == INT_MAX) continue;
      else count++;
   }

   int distarray[count];
   int i = 0;
   for(int a = 0; a < count; a++){
      for(i; i < _table.size(); i++){
         if(_table[i][c] == INT_MAX) continue;
         else {distarray[a] = _table[i][c]; i++; break;}
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
   return dist;
}

float
DBTable::getAve(size_t c) const
{
   // TODO: compute the average of data in column #c
   float sum = getSum(c);
   bool emptyCol = false;
   int count = 0;
   for(int i = 0; i < _table.size(); i++)
   {
     if(_table[i][c] != INT_MAX) {count++; emptyCol = true;} 
   }
   if(emptyCol) return (sum/count);
   else return NAN;
}

void
DBTable::sort(const struct DBSort& s)
{
   // TODO: sort the data according to the order of columns in 's'
   std::sort(_table.begin(),_table.end(),s);
}

void
DBTable::printCol(size_t c) const
{
   // TODO: to print out a column.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
   for(int i = 0; i < _table.size(); i++)
   {
     if(_table[i][c] == INT_MAX) cout<<".";
     else cout<<_table[i][c];
     if(i != (_table.size()-1)) cout<<" ";
   }

}

void
DBTable::printSummary() const
{
   size_t nr = nRows(), nc = nCols(), nv = 0;
   for (size_t i = 0; i < nr; ++i)
      for (size_t j = 0; j < nc; ++j)
         if (_table[i][j] != INT_MAX) ++nv;
   cout << "(#rows, #cols, #data) = (" << nr << ", " << nc << ", "
        << nv << ")" << endl;
}

