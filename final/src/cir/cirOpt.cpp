/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
// Remove unused gates
// DFS list should NOT be changed
// UNDEF, float and unused list may be changed
void
CirMgr::sweep()
{
  for(int i = 0; i < _AIGList.size(); i++){
    bool NotUse = true;
    for(int j = 0; j < _NetList.size(); j++){
      if(_AIGList[i]->getID() == _NetList[j]->getID()){
        NotUse = false;
      break;
      }
    }

    if(NotUse){
      //fanin     
      GateList& temp = _AIGList[i]->getFanin();
      for(int j = 0; j < temp.size(); j++){
        //fanin is Undef gate which has only one fanout--delete Undef
        if(temp[j]->getType() == UNDEF_GATE && (temp[j]->getFanout()).size() == 1){
          unsigned id = temp[j]->getID();
          for(int a = 0; a < _UndefList.size(); a++){
            if(_UndefList[a]->getID() == id){
              _UndefList.erase(_UndefList.begin()+a);
              break;
            } 
          }
          for(int a = 0; a < _totalID.size(); a++){
            if(_totalID[a] == id){
              _totalID.erase(_totalID.begin()+a);
              break;
            }  
          }
          delete temp[j];
          cout<<"Sweeping: UNDEF("<<temp[j]->getID()<<") removed..."<<endl;
        }
        else{
          GateList& fanout = temp[j]->getFanout();
          for(int a = 0; a < fanout.size(); a++){
            if(fanout[a]->getID() == _AIGList[i]->getID()){
              fanout.erase(fanout.begin()+a);
              break;
            }
          }
        }
      }
      temp = _AIGList[i]->getFanout();
      for(int j = 0; j < temp.size(); j++){
        GateList& fanin = temp[j]->getFanin();
        for(int a = 0; a < fanin.size(); a++){
          if(fanin[a]->getID() == _AIGList[i]->getID()){
            fanin.erase(fanin.begin()+a);
            break;
          }
        }
      }
      cout<<"Sweeping: AIG("<<_AIGList[i]->getID()<<") removed..."<<endl;
      delete _AIGList[i];
      _AIGList[i] = NULL;
    }   
  }

  for(int i = 0; i < _AIGList.size();){
    if(_AIGList[i] == NULL) _AIGList.erase(_AIGList.begin()+i);
    else i++;
  }

}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...
void
CirMgr::optimize()
{

  for(int i = 0; i < _NetList.size(); i++){
    if(_NetList[i]->isAig()){
      GateList& in = _NetList[i]->getFanin();
      vector<bool> b = _NetList[i]->AIGinverted();
      if(in[0]->getID() == 0 ){
        if(b[0]){
          if(b[1]){
            cout<<"Simplifying: "<<in[1]->getID()<<" merging !"<<_NetList[i]->getID()<<"..."<<endl;
            replaceInvert(in[1], _NetList[i]);
          }
          else{
            cout<<"Simplifying: "<<in[1]->getID()<<" merging "<<_NetList[i]->getID()<<"..."<<endl;
            replace(in[1], _NetList[i]);
          }        
        }
        else{
          cout<<"Simplifying: "<<in[0]->getID()<<" merging "<<_NetList[i]->getID()<<"..."<<endl;
          replace(in[0], _NetList[i]);
        }
      }
      else if(in[1]->getID() == 0 && in[0]->getID() != 0){
        if(b[1]){
          if(b[0]){
            cout<<"Simplifying: "<<in[0]->getID()<<" merging !"<<_NetList[i]->getID()<<"..."<<endl;
            replaceInvert(in[0], _NetList[i]);
          }
          else{
            cout<<"Simplifying: "<<in[0]->getID()<<" merging "<<_NetList[i]->getID()<<"..."<<endl;
            replace(in[0], _NetList[i]);
          }
        }
        else{
          cout<<"Simplifying: "<<in[1]->getID()<<" merging "<<_NetList[i]->getID()<<"..."<<endl;
          replace(in[1], _NetList[i]);
        }
      }
      else{
        if(in[0]->getID() == in[1]->getID()){
          if(b[0] == b[1]){
            if(b[0]){
              cout<<"Simplifying: "<<in[0]->getID()<<" merging !"<<_NetList[i]->getID()<<"..."<<endl;
              replaceInvert(in[0], _NetList[i]);
            }
            else{
              cout<<"Simplifying: "<<in[0]->getID()<<" merging "<<_NetList[i]->getID()<<"..."<<endl;
              replace(in[0], _NetList[i]);
            }
          }
          else{
            cout<<"Simplifying: 0 merging "<<_NetList[i]->getID()<<"..."<<endl;
            if(_ConstList.empty()){
              CirGate* con = new CirConst(0,0);
              _ConstList.push_back(con);
              _totalID.push_back(0);
              replace(con, _NetList[i]);
            }
            else replace(_ConstList[0],_NetList[i]);
          }
        }
      } 
    }
  }
  setNetlist();
}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/
void
CirMgr::replace(CirGate* remain, CirGate* del)
{
  GateList& delfanout = del->getFanout();
  GateList& fanout = remain->getFanout();
  for(int i = 0; i < delfanout.size(); i++){
    fanout.push_back(delfanout[i]);
  }
  for(int i = 0; i < delfanout.size(); i++){
    GateList& fanin = delfanout[i]->getFanin();
    for(int j = 0; j < fanin.size(); j++){
      if(fanin[j]->getID() == del->getID()){
        fanin[j] = remain;
        break;
      }
    }
  }
  GateList& delfanin = del->getFanin();
  for(int i = 0; i < delfanin.size(); i++){
    GateList& fanout2 = delfanin[i]->getFanout();
    for(int j = 0; j < fanout2.size(); j++){
      if(fanout2[j]->getID() == del->getID()){
        fanout2.erase(fanout2.begin()+j);
        break;
       }
     }
   }

  for(int i = 0; i < _AIGList.size(); i++){
    if(_AIGList[i]->getID() == del->getID()){
      delete _AIGList[i];
      _AIGList.erase(_AIGList.begin()+i);
      break;
    }
  }
}

void
CirMgr::replaceInvert(CirGate* remain, CirGate* del)
{
  GateList& delfanout = del->getFanout();
  GateList& fanout = remain->getFanout();
  for(int i = 0; i < delfanout.size(); i++){
    fanout.push_back(delfanout[i]);
  }
  for(int i = 0; i < delfanout.size(); i++){
    GateList& fanin = delfanout[i]->getFanin();
    if(delfanout[i]->isAig()){
      vector<bool>& b = delfanout[i]->AIGinverted();
      for(int j = 0; j < fanin.size(); j++){
        if(fanin[j]->getID() == del->getID()){
        fanin[j] = remain;
        if(b[j]) b[j] = false;
        else b[j] = true;
        break;
        }
      }
    }
    else{
      bool& b = delfanout[i]->inverted();
      fanin[0]= remain;
      if(b) b =false;
      else b = true;
    }
  }
  GateList& delfanin = del->getFanin();
  for(int i = 0; i < delfanin.size(); i++){
    GateList& fanout2 = delfanin[i]->getFanout();
    for(int j = 0; j < fanout2.size(); j++){
      if(fanout2[j]->getID() == del->getID()){
        fanout2.erase(fanout2.begin()+j);
        break;
       }
     }
   }

  for(int i = 0; i < _AIGList.size(); i++){
    if(_AIGList[i]->getID() == del->getID()){
      delete _AIGList[i];
      _AIGList.erase(_AIGList.begin()+i);
      break;
    }
  }
}