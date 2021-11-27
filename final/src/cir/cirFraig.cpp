/****************************************************************************
  FileName     [ cirFraig.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir FRAIG functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
#include "myHashMap.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::strash()" and "CirMgr::fraig()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/
class HashData{
public:
    HashData(){}
    HashData(CirGate* g):Gate(g) {}
    void merge( HashData& gate ) {
      cout << "Strashing: " << Gate->getID() << " merging " 
         << gate.Gate->getID() << "..." << endl;
      cirMgr->replace(Gate, gate.Gate);
    }
private:
    CirGate* Gate;      
};
/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/*******************************************/
/*   Public member functions about fraig   */
/*******************************************/
// _floatList may be changed.
// _unusedList and _undefList won't be changed
void
CirMgr::strash()
{
  size_t aig;
  for(int i = 0; i <_NetList.size(); i++){
    if(_NetList[i]->isAig()) aig++;
  }
  HashMap<HashKey<CirGate>, HashData> hash(aig);
  for(int i = 0; i < _NetList.size(); i++){
    if(_NetList[i]->isAig()){
      GateList& temp = _NetList[i]->getFanin();
      vector<bool> b = _NetList[i]->AIGinverted();
      HashKey<CirGate> k(temp, b);
      HashData g(_NetList[i]), g1;
      if(hash.check(k, g1) == true){
        g1.merge(g);
      } 
      else hash.forceInsert(k,g);
    }
  }

  setNetlist();
}

void
CirMgr::fraig()
{
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/
