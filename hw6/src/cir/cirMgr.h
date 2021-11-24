/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
   CirMgr(){}
   ~CirMgr() {}

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned) const;

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) const;

private:
   int           _M;
   int           _I;
   int           _O;
   int           _A; 
   GateList      _PIList;
   GateList      _POList;
   GateList      _AIGList;
   GateList      _ConstList;
   GateList      _UndefList;
   GateList      _netlist;
   IdList        _totalID;
   bool getMILOA(const string&);
   bool getAIG(const string&,int&,int&,int&);
   bool checkIDList(const int&);
   void setAIGfanin(const int&, CirGate*);
   bool checkUndefList(const int&);
   bool getsymbol(const string&);
   void setnetlist(CirGate*);
   void clear();
};

#endif // CIR_MGR_H
