/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"
#include "sat.h"

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
class CirGate
{
public:
   CirGate(){}
   CirGate(unsigned id, unsigned l,GateType t):
      _ID(id), _line(l), _type(t), _visited(true), _simulate(0){} 
   virtual ~CirGate() {}

   // Basic access methods
   virtual string getTypeStr() const {}
   unsigned getLineNo() const { return _line; }
   virtual bool isAig() const { return false; }

   // Printing functions
   virtual void printGate() const {}
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;

   unsigned getID() const{return _ID;}
   GateType getType() const{return _type;}
   bool getvisited() const{return _visited;}
   void setvisited() {_visited = false;}
   void boolclear() {_visited = true;}
   virtual string getSymbol() const{return "";}
   virtual void setSymbol(const string&){}
   virtual void setFanin(CirGate*){}
   virtual void setFanout(CirGate*){}
   virtual void setFanout(GateList){}
   virtual GateList& getFanin(){}
   virtual GateList& getFanout(){}
   virtual bool& inverted(){}
   virtual vector<bool>& AIGinverted(){}
   void setSim(unsigned long long sim){_simulate = sim;}
   unsigned long long getSim() const{return _simulate;}
   virtual unsigned long long Simulate() {}
   virtual GateList& getFecgroup(){}
   virtual void setFECgroup(GateList g){}

private:
   
protected:
   bool          _visited;
   unsigned      _ID;
   unsigned      _line;
   unsigned long long      _simulate;
   GateType      _type;

};

class CirPI : public CirGate
{
public:
  CirPI(unsigned id, unsigned l):
     CirGate(id, l, PI_GATE), _symbol(""){}
  ~CirPI(){}
  string getTypeStr() const{return "PI";}
  void printGate() const;
  void setFanout(CirGate* out){_fanout.push_back(out);}
  void setSymbol(const string& name){_symbol = name;}
  string getSymbol() const{return _symbol;}
  GateList& getFanout() {return _fanout;}
private:
   string        _symbol;
   GateList      _fanout;
};

class CirPO : public CirGate
{
public:
  CirPO(unsigned id, unsigned l, bool i):
     CirGate(id, l, PO_GATE), _inverted(i), _symbol(""){}
  ~CirPO(){}
  string getTypeStr() const{return "PO";}
  void printGate() const;
  void setFanin(CirGate* in){_fanin.push_back(in);}
  void setSymbol(const string& name){_symbol = name;}
  string getSymbol() const{return _symbol;}
  GateList& getFanin() {return _fanin;}
  bool& inverted(){return _inverted;}
  GateList& getFECgroup(){return _FECgroup;}
  void setFECgroup(GateList g){_FECgroup = g;}
private:
   bool         _inverted;
   string       _symbol;
   GateList     _FECgroup;
   GateList     _fanin;
};

class CirAIG : public CirGate
{
public:
  CirAIG(unsigned id, unsigned l, bool i1, bool i2):
     CirGate(id, l, AIG_GATE)
     {_inverted.push_back(i1);_inverted.push_back(i2);}
  ~CirAIG(){}
  string getTypeStr() const{return "AIG";}
  bool isAig() const{return true;}
  void printGate() const;
  void setFanin(CirGate* in){_fanin.push_back(in);}
  void setFanout(CirGate* out){_fanout.push_back(out);}
  void setFanout(GateList out){_fanout = out;}
  GateList& getFanin() {return _fanin;}
  GateList& getFanout() {return _fanout;}
  vector<bool>& AIGinverted(){return _inverted;}
  unsigned long long Simulate();
  GateList& getFECgroup(){return _FECgroup;}
  void setFECgroup(GateList g){_FECgroup = g;}
private:
   GateList      _fanin;
   GateList      _fanout;
   GateList      _FECgroup;
   vector<bool>  _inverted;
};

class CirConst : public CirGate
{
public:
  CirConst(unsigned id, unsigned l):
     CirGate(id, l, CONST_GATE){}
  ~CirConst(){}
  string getTypeStr() const{return "CONST";}
  void printGate() const;
  void setFanout(CirGate* out){_fanout.push_back(out);}
  GateList& getFanout() {return _fanout;}
private:
   GateList      _fanout;
};

class CirUndef : public CirGate
{
public:
  CirUndef(unsigned id, unsigned l):
     CirGate(id, l, UNDEF_GATE){}
  ~CirUndef(){}
  string getTypeStr() const{return "UNDEF";}
  void setFanout(CirGate* out){_fanout.push_back(out);}
  GateList& getFanout() {return _fanout;}
private:
   GateList      _fanout;
};

#endif // CIR_GATE_H
