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

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

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
  cout<<"=================================================="<<endl;
  if(_symbol !=""){
    string ouput = "";
    ouput = ouput+"= "+getTypeStr()+"("+id+")\""+_symbol+"\", line "+l;
    cout<<setw(49)<<left<<ouput<<"="<<endl;
  }
  else{
    string ouput = "";
    ouput = ouput+"= "+getTypeStr()+"("+id+"), line "+l;
    cout<<setw(49)<<left<<ouput<<"="<<endl;    
  }
  cout<<"=================================================="<<endl;
}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   cout<<getTypeStr()<<" "<<_ID<<endl;
   level--;
   for(int i = 0; i < _faninList.size(); i++){
     int index = 0;
     printfanin(_faninList[i],index,level,_invertedlist[i]);
   }
   for(int i = 0; i < _faninList.size(); i++){
     clearfanin(_faninList[i]);
   }
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
   cout<<getTypeStr()<<" "<<_ID<<endl;
   level--;
   for(int i = 0; i < _fanoutList.size(); i++){
     int index = 0;
     printfanout(_fanoutList[i],index,level,_ID);
   }
   for(int i = 0; i < _fanoutList.size(); i++){
     clearfanout(_fanoutList[i]);
   }
}

void
CirGate::printfanin(CirGate* input , int index ,const int& level, bool invert)const
{
  if(index <= level){

  
  if(input->getvisited()){
    input->setvisited();
    for(int i = 0; i < 2*index+2; i++){
      cout<<" ";
    }
    if(invert) cout<<"!";
    cout<<input->getTypeStr()<<" "<<input->getID()<<endl;
    GateList& temp = input->getfanin();
    index ++;
    for(int i = 0; i < temp.size(); i++){
        printfanin(temp[i],index,level,(input->getinvert())[i]);
    }
  }
  else{
      for(int i = 0; i < 2*index+2; i++){
        cout<<" ";
      }
      if(invert) cout<<"!";
      cout<<input->getTypeStr()<<" "<<input->getID();
      if(input->getType() != PI_GATE)
      cout<<" (*)";
      cout<<endl;      
    }

  }
}

void
CirGate::clearfanin(CirGate* input) const
{
  input->boolclear();
   GateList& temp = input->getfanin();
    for(int i = 0; i < temp.size(); i++){
      clearfanin(temp[i]);
    }
}

void
CirGate::printfanout(CirGate* input , int index ,const int& level,const int& id)const
{
  if(index <= level){

  
  if(input->getvisited()){
    input->setvisited();
    for(int i = 0; i < 2*index+2; i++){
      cout<<" ";
    }
    for(int i = 0; i < (input->getfanin()).size(); i++){
      if((input->getfanin())[i]->getID() == id)
         if((input->getinvert())[i])
            cout<<"!";
    }
    cout<<input->getTypeStr()<<" "<<input->getID()<<endl;
    GateList& temp = input->getfanout();
    index ++;
    for(int i = 0; i < temp.size(); i++){
        printfanout(temp[i],index,level,input->getID());
    }
  }
  else{
      for(int i = 0; i < 2*index+2; i++){
        cout<<" ";
      }
      for(int i = 0; i < (input->getfanin()).size(); i++){
        if((input->getfanin())[i]->getID() == id)
          if((input->getinvert())[i])
            cout<<"!";
    }
      cout<<input->getTypeStr()<<" "<<input->getID();
      if(input->getType() != PO_GATE)
      cout<<" (*)";
      cout<<endl;      
    }

  }
}

void
CirGate::clearfanout(CirGate* input) const
{
  input->boolclear();
   GateList& temp = input->getfanout();
    for(int i = 0; i < temp.size(); i++){
      clearfanin(temp[i]);
    }
}

void
CirPI::printGate()const
{
  cout<<"PI  "<<_ID;
  if(_symbol != "")
     cout<<" ("<<_symbol<<")";
  cout<<endl;
}

void
CirPO::printGate()const
{
  cout<<"PO  "<<_ID<<" ";
  if(_faninList[0]->getType() == UNDEF_GATE) cout<<"*";
  if(_invertedlist[0]) cout<<"!";
  cout<<_faninList[0]->getID();
  if(_symbol != "")
     cout<<" ("<<_symbol<<")";
  cout<<endl;
}

void
CirAIG::printGate()const
{
  cout<<"AIG "<<_ID;
  for(int i = 0; i < _faninList.size(); i++){
    cout<<" ";
    if(_faninList[i]->getType() == UNDEF_GATE) cout<<"*";
    if(_invertedlist[i]) cout<<"!";
    cout<<_faninList[i]->getID();
  }
  cout<<endl;
}

void
CirConst::printGate()const
{
  cout<<"CONST0"<<endl;
}

