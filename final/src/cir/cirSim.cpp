/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir simulation functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"
#include <sstream>
#include <bitset>
#include <math.h>

using namespace std;

// TODO: Keep "CirMgr::randimSim()" and "CirMgr::fileSim()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/
static int Size = sizeof(unsigned long long)*8;
/**************************************/
/*   Static varaibles and functions   */
/**************************************/
bool
SortFecGrp(CirGate* a, CirGate* b)
{
  return(a->getID() < b->getID());
}
/************************************************/
/*   Public member functions about Simulation   */
/************************************************/
void
CirMgr::randomSim()
{
  iniFECList();
}

void
CirMgr::fileSim(ifstream& patternFile)
{
  iniFECList();
  vector<unsigned> sim;
  string temp;
  vector<string> input;
  while(patternFile>>temp)
     input.push_back(temp);
  size_t counter = 0;
  if(checkPattern(input)){
    for(int i = 0; i < input.size(); i+=Size){
      setPIsim(input,counter);
      for(int j = 0; j < _NetList.size(); j++){
        if(_NetList[j]->isAig()){
          _NetList[j]->setSim(_NetList[j]->Simulate());
        }
      }
      counter++;
      setFECList();
    }
  }

  for (int i = 0; i < _FECList.size() ; ++i )  
    sort(_FECList[i] .begin(), _FECList[i].end(), SortFecGrp);
  cout<<input.size()<<" patterns simulated."<<endl;
}

/*************************************************/
/*   Private member functions about Simulation   */
/*************************************************/
void
CirMgr::iniFECList()
{
  if(_FECList.empty()){
    GateList fec;
    if(!_ConstList.empty())
       fec.push_back(_ConstList[0]);
    for(int i = 0; i < _NetList.size(); i++){
      if(_NetList[i]->isAig())
        fec.push_back(_NetList[i]);
    }
    _FECList.push_back(fec);
  }
}

bool
CirMgr::checkPattern(vector<string>& input)
{
  for(int i = 0; i < input.size(); i++){
    if(input[i].size() != _PIList.size()){
      cout<<"Error: Pattern("<<input[i]<<") length("<<input[i].size()
      <<") does not match the number of inputs("<<_PIList.size()
      <<") in a circuit!!";
      return false;
    }
  }
  for(int i = 0; i < input.size(); i++){
    for(int j = 0; j < input[i].size(); j++){
      if(input[i][j] != '0' && input[i][j] != '1'){
        cout<<"Error: Pattern("<<input[i]<<") contains a non-0/1 character ('"
        <<input[i][j]<<"').";
        return false;
      }
    }
  }
  return true;
}

void 
CirMgr::setPIsim(vector<string>& input, size_t& counter)
{
  size_t num;
  size_t done = counter*Size;
  if(input.size()-done > Size) num = Size;
  else num = input.size();
  string pattern = "";
  stringstream p;
  for(int i = 0; i < _PIList.size(); i++){
    for(size_t j = done; j < done+num; j++){
      pattern += input[j][i];
    }
    bitset< sizeof(unsigned long long)*8 > b(pattern);
    unsigned long long temp = b.to_ulong();
    p << hex << temp;
    _PIList[i]->setSim(temp);
    p.str("");
    p.clear();
    pattern = "";
  }

}

void
CirMgr::setFECList() 
{
  vector<GateList> tempFEC;
  for (int i = 0; i < _FECList.size(); i++) {
    HashMap< HashKey<CirGate>, CirGate*> newFec(_FECList[i].size());
    for (int j = 0; j < _FECList[i].size(); j++) {
      HashKey<CirGate> k(_FECList[i][j]->getSim() );
      CirGate* grp;
      if (newFec.check(k, grp)){/*    
        (grp->getFecgroup()).push_back(_FECList[i][j] );
        _FECList[i][j]->setFECgroup(grp->getFecgroup());*/
        cout<<"@";
      }
      else {/*
        GateList temp;
        _FECList[i][j]->setFECgroup(temp);
        temp.push_back(_FECList[i][j]);
        tempFEC.push_back(temp);
        newFec.forceInsert(k, _FECList[i][j]);*/
        cout<<"!";
      }
    }
  }
  _FECList.clear();
  _FECList = tempFEC;
}