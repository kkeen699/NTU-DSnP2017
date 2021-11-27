/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

// TODO: Keep "CirGate::reportGate()", "CirGate::reportFanin()" and
//       "CirGate::reportFanout()" for cir cmds. Feel free to define
//       your own variables and functions.

extern CirMgr *cirMgr;

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
  stringstream ss;
  ss << _ID;
  string id = ss.str();
  ss.str( "" );
  ss << _line;
  string l = ss.str();
  cout<<"================================================================================"<<endl;
  if(getSymbol() != ""){
    string ouput = "";
    ouput = ouput+"= "+getTypeStr()+"("+id+")\""+getSymbol()+"\", line "+l;
    cout<<setw(49)<<left<<ouput<<endl;
  }
  else{
    string ouput = "";
    ouput = ouput+"= "+getTypeStr()+"("+id+"), line "+l;
    cout<<setw(49)<<left<<ouput<<endl;    
  }
  cout << setw(49) << left << "= FECs:"<< endl;
  cout << "= Value: "; 
  size_t s = 0x80000000;
  for ( int i = 0 ; i < 64 ; ++i ) {
    if ( ( ( i % 8 ) == 0 ) && ( i != 0 ) )
      cout << "_";
    cout << "0";
    s = s >> 1;
  }
  cout<< endl;
  cout<<"================================================================================"<<endl;  
}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
}

void
CirPI::printGate() const
{
  cout<<"PI  "<<_ID;
  if(_symbol != "")
     cout<<" ("<<_symbol<<")";
  cout<<endl;
}

void
CirPO::printGate() const
{
  cout<<"PO  "<<_ID<<" ";
  if(_fanin[0]->getType() == UNDEF_GATE) cout<<"*";
  if(_inverted) cout<<"!";
  cout<<_fanin[0]->getID();
  if(_symbol != "")
     cout<<" ("<<_symbol<<")";
  cout<<endl;
}

void
CirAIG::printGate() const
{
  cout<<"AIG "<<_ID;
  for(int i = 0; i < _fanin.size(); i++){
    cout<<" ";
    if(_fanin[i]->getType() == UNDEF_GATE) cout<<"*";
    if(_inverted[i]) cout<<"!";
    cout<<_fanin[i]->getID();
  }
  cout<<endl;
}

void
CirConst::printGate() const
{
  cout<<"CONST0"<<endl;
}

unsigned long long
CirAIG::Simulate()
{
  unsigned long long in1 = _fanin[0]->getSim();
  unsigned long long in2 = _fanin[1]->getSim();
  if(_inverted[0]) in1 = ~in1;
  if(_inverted[1]) in2 = ~in2;
  return in1 & in2;
}
