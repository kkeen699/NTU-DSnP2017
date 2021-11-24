/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include <sstream>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"
#include <algorithm>

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool
CirMgr::readCircuit(const string& fileName)
{
   fstream file(fileName);
   if(!file){
     cerr<<"Can't open design \""<<fileName<<"\"!!";
     return false;
   }
   string input;
   getline(file,input,'\n');
   if(!getMILOA(input))  return false;
   lineNo++;

   //PI
   for(int i = 0; i < _I; i++){
     getline(file,input,'\n');
     int id;
     if(!myStr2Int(input,id)){
       errMsg = input;
       parseError(ILLEGAL_NUM);
       return false;
     }
     if(id > _M){
       errMsg = "PI";
       errInt = id;
       parseError(NUM_TOO_BIG);
       return false;
     }
     lineNo++;
     CirGate* pi = new CirPI(id/2,lineNo);
     _PIList.push_back(pi);
     _totalID.push_back(id/2);
   }

   //PO
   for(int i = 0; i < _O; i++){
     getline(file,input,'\n');
     int id;
     if(!myStr2Int(input,id)){
       errMsg = input;
       parseError(ILLEGAL_NUM);
       return false;
     }
     lineNo++;
     bool inverted = false;
     if(id%2 != 0) inverted = true;
     CirGate* po = new CirPO(_M+1+i,lineNo,inverted);
     if(id/2 == 0){
       CirGate* con = new CirConst(0,lineNo);
       con->setfanout(po);
       po->setfanin(con);
       _POList.push_back(po);
       _totalID.push_back(_M+1+i);
       _ConstList.push_back(con);
       _totalID.push_back(0);
     }
     else{
       CirGate* undef = new CirUndef(id/2,lineNo);
       undef->setfanout(po);
       po->setfanin(undef);     
       _POList.push_back(po);
       _totalID.push_back(_M+1+i);
       _UndefList.push_back(undef);
       _totalID.push_back(id/2);
     }
   }

   //AIG
   for(int i = 0; i < _A; i++){
     getline(file,input,'\n');
     int id,fanin1,fanin2;
     if(!getAIG(input,id,fanin1,fanin2)) return false;
     lineNo++;

     bool inverted1 = false;
     if(fanin1%2 != 0) inverted1 = true;
     bool inverted2 = false;
     if(fanin2%2 != 0) inverted2 = true;

     CirGate* aig = new CirAIG(id/2,lineNo,inverted1,inverted2);
     _AIGList.push_back(aig);
     _totalID.push_back(id/2);

     //set fanin
     setAIGfanin(fanin1/2,aig);
     setAIGfanin(fanin2/2,aig);

     // set fanout
     if(checkUndefList(id/2)){
       for(int i = 0; i < _UndefList.size(); i++){
         if(_UndefList[i]->getID() == id/2){
           GateList& Outtemp = _UndefList[i]->getfanout();
           aig->setfanout(_UndefList[i]->getfanout());
           for(int x = 0; x < Outtemp.size(); x++){
             GateList& Intemp = Outtemp[x]->getfanin();
             for(int y = 0; y < Intemp.size(); y++){
               if(Intemp[y] == _UndefList[i]){
                 Intemp[y] = aig;
                 break;
               }
             }
           }
           delete _UndefList[i];
           _UndefList.erase(_UndefList.begin()+i);
           break;
         }
       }
     }
   }
   //set symbol
   while(getline(file,input,'\n')){
     if(input.size()==1 && input[0] == 'c') break;
     getsymbol(input);
   }

  for(int i = 0; i < _POList.size(); i++){
    setnetlist(_POList[i]);
  }
  clear();
   return true;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
  cout<<"Circuit Statistics"<<endl;
  cout<<"=================="<<endl;
  cout<<"  PI"<<setw(12)<<right<< _I <<endl;
  cout<<"  PO"<<setw(12)<<right<< _O <<endl;
  cout<<"  AIG"<<setw(11)<<right<< _A <<endl;
  cout<<"------------------"<<endl;
  cout<<"  Total"<<setw(9)<<right<< _I+_O+_A <<endl;
}

void
CirMgr::printNetlist() const
{

  for(int i = 0; i < _netlist.size(); i++){
    cout<<"["<<i<<"] ";
    _netlist[i]->printGate();
  }
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit:";
   for(int i = 0; i < _PIList.size(); i++){
     cout<<" "<<_PIList[i]->getID();
   }
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit:";
   for(int i = 0; i < _POList.size(); i++){
     cout<<" "<<_POList[i]->getID();
   }
   cout << endl;
}

void
CirMgr::printFloatGates() const
{

  vector<unsigned>  _floating;
  for(int i = 0; i < _AIGList.size(); i++){
    if(_AIGList[i]->checkFanin() != 0)
       _floating.push_back(_AIGList[i]->checkFanin());
  }
  for(int i = 0; i < _POList.size(); i++){
    if(_POList[i]->checkFanin() != 0)
       _floating.push_back(_POList[i]->checkFanin());
  }
  if(!_floating.empty()){
    cout<<"Gates with floating fanin(s):";
    sort( _floating.begin(), _floating.end() );
    for(int i = 0; i < _floating.size(); i++){
      cout<<" "<<_floating[i];
    }
    cout<<endl;
  }

  vector<unsigned>  _DBNU;
  for(int i = 0; i < _AIGList.size(); i++){
    if(_AIGList[i]->checkFanout() != 0)
       _DBNU.push_back(_AIGList[i]->checkFanout());
  }
  for(int i = 0; i < _PIList.size(); i++){
    if(_PIList[i]->checkFanout() != 0)
       _DBNU.push_back(_PIList[i]->checkFanout());
  }
  if(!_DBNU.empty()){
    cout<<"Gates defined but not used  :";
    sort( _DBNU.begin(), _DBNU.end() );
    for(int i = 0; i < _DBNU.size(); i++){
      cout<<" "<<_DBNU[i];
    }
    cout<<endl;
  }
}

void
CirMgr::writeAag(ostream& outfile) const
{
  outfile << "aag " << _M 
          << " " << _I
          << " 0"
          << " " << _O
          << " " << _A << endl;
  for ( int i = 0 ; i < _PIList.size() ; i++ ) 
    outfile << 2*(_PIList[ i ]->getID()) << endl;

  for ( int i = 0 ; i < _POList.size() ; i++ ){
    size_t id =(_POList[i]->getfanin())[0]->getID();
    if((_POList[i]->getinvert())[0])
       outfile<<2*id+1<<endl;
    else
       outfile<<2*id<<endl;
  }

  for(int i = 0; i <_AIGList.size(); i++){
    outfile<<2*(_AIGList[i]->getID())<<" ";
    size_t id1 =(_AIGList[i]->getfanin())[0]->getID();
    if((_AIGList[i]->getinvert())[0])
       outfile<<2*id1+1<<" ";
    else
       outfile<<2*id1<<" ";
    size_t id2 =(_AIGList[i]->getfanin())[1]->getID();
    if((_AIGList[i]->getinvert())[1])
       outfile<<2*id2+1<<endl;
    else
       outfile<<2*id2<<endl;        
  }  

  for ( int i = 0 ; i < _PIList.size() ; i++ ){
    if(_PIList[i]->getsymbol() != "")
       outfile<<"i"<<i<<" "<<_PIList[i]->getsymbol()<<endl;
  }
  for ( int i = 0 ; i < _POList.size() ; i++ ){
    if(_POList[i]->getsymbol() != "")
       outfile<<"o"<<i<<" "<<_POList[i]->getsymbol()<<endl;
  }

  outfile << 'c' << endl;
  outfile << "AAG output by Cheng-Yun Cheng" << endl;
}

CirGate*
CirMgr:: getGate(unsigned gid)const
{
  //constlist
  if(gid == 0){
    if(!_ConstList.empty()) return _ConstList[0];
  }

  //PIlist
  for(int i = 0; i < _PIList.size(); i++){
    if(_PIList[i]->getID() == gid) return _PIList[i];
  }

  //POlist
  for(int i = 0;i < _POList.size(); i++){
    if(_POList[i]->getID() == gid) return _POList[i];
  }

  //AIGlist
  for(int i = 0; i <_AIGList.size(); i++){
    if(_AIGList[i]->getID() == gid) return _AIGList[i];
  }

  //UndefList
  for(int i = 0; i <_UndefList.size(); i++){
    if(_AIGList[i]->getID() == gid) return _UndefList[i];
  }
  return 0;
}

bool
CirMgr::getMILOA(const string& input)
{
  vector<string> options;
  istringstream MILOA(input);
  string temp;
  while(getline(MILOA,temp,' ')){
    if(static_cast<int>(temp.back()) == 13) temp.pop_back();
    options.push_back(temp);
  }

  if(options[0].size() > 3 || myStrNCmp("aag", options[0],3) != 0){
    errMsg = options[0];
    parseError(ILLEGAL_IDENTIFIER);
    return false;
  }
  if(!myStr2Int(options[1],_M)) {
    errMsg = options[1];
    parseError(ILLEGAL_NUM);
    return false;}
  if(!myStr2Int(options[2],_I)){
    errMsg = options[2];
    parseError(ILLEGAL_NUM);
    return false;}  
  if(!myStr2Int(options[4],_O)){
    errMsg = options[4];
    parseError(ILLEGAL_NUM);
    return false;}
  if(!myStr2Int(options[5],_A)){
    errMsg = options[5];
    parseError(ILLEGAL_NUM);
    return false;}

  if(_M<_I+_A){
    errMsg = "Number of variables";
    errInt = _M;
    parseError(NUM_TOO_SMALL);
    return false;
  }    

  return true;
}

bool
CirMgr::getAIG(const string& input,int& id,int& fanin1,int& fanin2)
{
  vector<string> options;
  istringstream AIG(input);
  string temp;
  while(getline(AIG,temp,' ')){
    if(static_cast<int>(temp.back()) == 13) temp.pop_back();
    options.push_back(temp);
  }
  if(!myStr2Int(options[0],id)) return false;
  if(!myStr2Int(options[1],fanin1)) return false;
  if(!myStr2Int(options[2],fanin2)) return false;

  return true;
}

bool
CirMgr::checkIDList(const int& id)
{
  for(int i = 0; i < _totalID.size(); i++){
    if(_totalID[i] == id){
      return true;
    }
  }
  return false;
}

void
CirMgr::setAIGfanin(const int& id,CirGate* aig)
{
     if(id == 0){
       if(_ConstList.empty()){
         CirGate* con = new CirConst(0,lineNo);
         _ConstList.push_back(con);
         _totalID.push_back(0);
       }
       _ConstList[0]->setfanout(aig);
       aig->setfanin(_ConstList[0]);
     }
     
     //find from PIlist
     else if(checkIDList(id)){
       bool NotfindID = true;
       for(int i = 0; i < _PIList.size(); i++){
         if(_PIList[i]->getID() == id){
           _PIList[i]->setfanout(aig);
           aig->setfanin(_PIList[i]);
           NotfindID = false;
         }
       }

       //find from AIGlist
       if(NotfindID){
         for(int i = 0; i < _AIGList.size(); i++){
           if(_AIGList[i]->getID() == id){
             _AIGList[i]->setfanout(aig);
             aig->setfanin(_AIGList[i]);
             NotfindID = false;
           }
         }
       }

       //find from Undeflist
       if(NotfindID){
         for(int i = 0; i < _UndefList.size(); i++){
           if(_UndefList[i]->getID() == id){
             _UndefList[i]->setfanout(aig);
             aig->setfanin(_UndefList[i]);
             NotfindID = false;
           }
         }
       }
     } 

     else{
       CirGate* undef = new CirUndef(id,lineNo);
       undef->setfanout(aig);
       aig->setfanin(undef);
       _totalID.push_back(id);
       _UndefList.push_back(undef);
     } 
}

bool
CirMgr::checkUndefList(const int& id)
{
  for(int i = 0; i < _UndefList.size(); i++){
    if(_UndefList[i]->getID() == id) return true;
  }
  return false;
}

bool
CirMgr::getsymbol(const string& input)
{
  vector<string> options;
  istringstream symbol(input);
  string temp;
  while(getline(symbol,temp,' ')){
    if(static_cast<int>(temp.back()) == 13) temp.pop_back();
    options.push_back(temp);
  }
  if(options[0].size() == 2){
    int index;
    if((options[0])[0] == 'i'){
      options[0].erase(options[0].begin());
      if(myStr2Int(options[0],index))
         _PIList[index]->setsymbol(options[1]);
    }
    else if((options[0])[0] == 'o'){
      options[0].erase(options[0].begin());
      if(myStr2Int(options[0],index))
         _POList[index]->setsymbol(options[1]);
    }
  }
}

void
CirMgr::setnetlist(CirGate* input)
{
  if(input->getvisited()){
    input->setvisited();
    GateList& temp = input->getfanin();
    for(int i = 0; i < temp.size(); i++){
      if(temp[i]->getvisited()) setnetlist(temp[i]);
    }
    if(input->getType() != UNDEF_GATE)
       _netlist.push_back(input);
  }
}

void
CirMgr::clear()
{
  for(int i = 0; i < _PIList.size(); i++){
    _PIList[i]->boolclear();
  }
  for(int i = 0; i < _POList.size(); i++){
    _POList[i]->boolclear();
  }
  for(int i = 0; i < _AIGList.size(); i++){
    _AIGList[i]->boolclear();
  }
  for(int i = 0; i < _UndefList.size(); i++){
    _UndefList[i]->boolclear();
  }
    for(int i = 0; i < _ConstList.size(); i++){
    _ConstList[i]->boolclear();
  }
}