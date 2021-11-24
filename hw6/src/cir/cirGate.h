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

using namespace std;

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
  friend class CirMgr;
public:
   CirGate() {}
   CirGate(unsigned id, unsigned l,GateType t):
      _ID(id),_line(l), _symbol(""),_type(t),_visited(true){}
   CirGate(unsigned id, unsigned l,GateType t,bool i):
      _ID(id),_line(l), _symbol(""),_type(t),_visited(true){_invertedlist.push_back(i);}
   CirGate(unsigned id, unsigned l,GateType t,bool i1, bool i2):
      _ID(id),_line(l), _symbol(""),_type(t),_visited(true)
      { _invertedlist.push_back(i1);_invertedlist.push_back(i2);}           
   virtual ~CirGate() {}

   // Basic access methods
   virtual string getTypeStr() const = 0;
   unsigned getLineNo() const { return _line; }
   
   // Printing functions
   virtual void printGate() const = 0;
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;

   unsigned getID(){return _ID;}
   GateList& getfanout(){return _fanoutList;}
   GateList& getfanin(){return _faninList;}
   void setfanin(CirGate* in){_faninList.push_back(in);}
   void setfanout(CirGate* out){_fanoutList.push_back(out);}
   void setfanout(GateList out){
     for(int i = 0; i < out.size();i++){
       _fanoutList.push_back(out[i]);
     }
   }
   void setsymbol(const string& name){_symbol = name;}
   string getsymbol()const{return _symbol;}
   GateType getType()const{return _type;}
   virtual unsigned checkFanin() = 0;
   virtual unsigned checkFanout() = 0;
   bool getvisited()const{return _visited;}
   void setvisited(){_visited = false;}
   void boolclear(){_visited = true;}
   vector<bool> getinvert()const{return _invertedlist;}
   void clearfanin(CirGate*)const;
   void clearfanout(CirGate*)const;
   
private:
   void printfanin(CirGate* , int, const int&,bool)const;
   void printfanout(CirGate* ,int, const int&,const int&)const;
protected:
   bool        _visited;
   unsigned    _ID;
   unsigned    _line;
   string      _symbol;
   GateType    _type;
   GateList    _faninList;
   GateList    _fanoutList;
   vector<bool>          _invertedlist;
};


class CirPI : public CirGate
{
public:  
   CirPI(){}
   CirPI(unsigned id, unsigned l):
      CirGate(id,l,PI_GATE){}
   ~CirPI(){}
   string getTypeStr()const{return "PI";}
   unsigned checkFanout(){
     if(_fanoutList.empty()) return _ID;
     return 0;
   }
   unsigned checkFanin(){}
   void printGate() const;
   

private:
   //fanout
};

class CirPO : public CirGate
{
public:  
  CirPO(){}
  CirPO(unsigned id, unsigned l, bool i):
     CirGate(id,l,PO_GATE,i){}
  ~CirPO(){}
  string getTypeStr()const{return "PO";}
  unsigned checkFanout(){}
  unsigned checkFanin(){
    if(_faninList[0]->getType() == UNDEF_GATE) return _ID;
    return 0;
  }
  void printGate() const;
private:
   //fanin
};

class CirAIG : public CirGate
{
public:  
  CirAIG(){}
  CirAIG(unsigned id, unsigned l,bool i1,bool i2):
     CirGate(id,l,AIG_GATE,i1,i2){}
  ~CirAIG(){}
  string getTypeStr()const{return "AIG";}
  unsigned checkFanin(){
    for(int i = 0; i < _faninList.size(); i++){
      if(_faninList[i]->getType() == UNDEF_GATE) return _ID;
    }
    return 0;
  }
  unsigned checkFanout(){
    if(_fanoutList.empty()) return _ID;
    return 0;
  }

  void printGate() const;
private:
   //fanin
   //fanout
};

class CirConst : public CirGate
{
public:  
  CirConst(){}
  CirConst(unsigned id, unsigned l):
     CirGate(id,l,CONST_GATE){}
  ~CirConst(){}
  string getTypeStr()const{return "CONST";}
  unsigned checkFanin(){}
  unsigned checkFanout(){}
  void printGate() const;
private:
   //fanout
};

class CirUndef : public CirGate
{
public:
  CirUndef(){}
  CirUndef(unsigned id, unsigned l):
     CirGate(id,l,UNDEF_GATE){}
  ~CirUndef(){}
  unsigned checkFanin(){}
  unsigned checkFanout(){}
  string getTypeStr()const{return "UNDEF";}
  void printGate() const{}
private:
   //fanout
};

#endif // CIR_GATE_H
