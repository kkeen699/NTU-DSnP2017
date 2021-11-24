/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>

using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ==" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
template <class Data>
class HashSet
{
public:
   HashSet(size_t b = 0) : _numBuckets(0), _buckets(0) { if (b != 0) init(b); }
   ~HashSet() { reset(); }

   // TODO: implement the HashSet<Data>::iterator
   // o An iterator should be able to go through all the valid Data
   //   in the Hash
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   class iterator
   {
      friend class HashSet<Data>;

   public:
      iterator(vector<Data>* b, size_t n, size_t nb):
        _buckets(b), _number(n), _numBuckets(nb){
        if(_number != -1){
          int counter = 0;
          bool done = false;
          for(int i = 0; i < _numBuckets; i++){
            for(int j = 0; j < _buckets[i].size(); j++){
              if(counter == _number){
                _table = i;
                _index = j;
                _data = _buckets[_table][_index];
                done = true;
                break;
              }
              counter++;
            }
            if(done) break;
          }
        }  
      }
      ~iterator(){}

      const Data& operator * () const { if(_number != -1)return _data; }

      iterator& operator ++ () {
        //end++(begin)
        if(_number == -1){
          for(int i = 0; i < _numBuckets; i++){
            if(!_buckets[i].empty()){
              _number = 0;
              _table = i;
              _index = 0;
              _data = _buckets[_table][_index];
              break;
            }    
          }      
        }
        // same bucket
        else if(_index < _buckets[_table].size()-1){
          _number++;
          _index++;
          _data = _buckets[_table][_index];
        }
        else {
          bool last = true;
          for(int i = _table+1; i < _numBuckets; i++){
            if(!_buckets[i].empty()){
              _number++;
              _table = i;
              _index = 0;
              _data = _buckets[_table][_index];
              last = false;
              break;
            }
          }
          if(last) _number = -1;
        }
        return *this;
      }

      iterator operator ++ (int){
        iterator temp = *this;
        ++(*this);
        return temp;
      }

      iterator& operator -- (){
        if(_number == -1){
          for(int i = _numBuckets-1; i > -1; i--){
            if(!_buckets[i].empty()){
              _table = i;
              _index = _buckets[i].size()-1;
              _data = _buckets[_table][_index];
              break;
            }
          }
          for(int i = 0; i < _numBuckets; i++){
            for(int j = 0; j < _buckets[i].size(); j++)
               _number++;
          }
        }        
        // same bucket
        else if(_index > 0){
          _number--;
          _index--;
          _data = _buckets[_table][_index];
        }
        else {
          bool first = true;
          for(int i = _table-1; i > -1; i--){
            if(!_buckets[i].empty()){
              _number--;
              _table = i;
              _index = _buckets[i].size()-1;
              _data = _buckets[_table][_index];
              first = false;
              //cout<<_buckets[i].size()-1<<endl;
              break;
            }
          }
          if(first) _number = -1;
        }
        return *this;
      }

      iterator operator -- (int){
        iterator temp = *this;
        --(*this);
        return temp;
      }
      iterator& operator = (const iterator& i){
        _number = i._number;
        _table = i._table;
        _index = i._index;
        _data = i._data;
      }
      bool operator == (const iterator& i) const{
        if(_number == i._number) return true;
        else return false;
      }
      bool operator != (const iterator& i) const {
        if(_number == i._number) return false;
        else return true;
      }
   private:
      size_t            _number;
      size_t            _table;
      size_t            _index;
      size_t            _numBuckets;
      Data              _data;
      vector<Data>*     _buckets;
   };

   void init(size_t b) { _numBuckets = b; _buckets = new vector<Data>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   void clear() {
      for (size_t i = 0; i < _numBuckets; ++i) _buckets[i].clear();
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<Data>& operator [] (size_t i) { return _buckets[i]; }
   const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   iterator begin() const {
     if(!empty()) return iterator(_buckets, 0, _numBuckets);
     return end();
   }
   // Pass the end
   iterator end() const { 
     return iterator(_buckets, -1, _numBuckets);
   }
   // return true if no valid data
   bool empty() const {
     for(int i = 0; i < _numBuckets; i++)
        if(!_buckets[i].empty()) return false;
     return true;
   }
   // number of valid data
   size_t size() const {
     size_t s = 0;
     for(int i = 0; i < _numBuckets; i++)
        s += _buckets[i].size();
     return s;
   }

   // check if d is in the hash...
   // if yes, return true;
   // else return false;
   bool check(const Data& d) const {
     size_t index = bucketNum(d);
     for(size_t i = 0; i < _buckets[index].size(); i++){
       if(_buckets[index][i] == d) return true;
     }
     return false;
   }

   // query if d is in the hash...
   // if yes, replace d with the data in the hash and return true;
   // else return false;
   bool query(Data& d) const { 
     if(check(d)){
       size_t index = bucketNum(d);
       for(size_t i = 0; i < _buckets[index].size(); i++){
         if(_buckets[index][i] == d){
           d = _buckets[index][i];
           return true;
         }
       }
     }
     return false;
   }

   // update the entry in hash that is equal to d (i.e. == return true)
   // if found, update that entry with d and return true;
   // else insert d into hash as a new entry and return false;
   bool update(const Data& d) { 
     if(check(d)){
       size_t index = bucketNum(d);
       for(size_t i = 0; i < _buckets[index].size(); i++){
         if(_buckets[index][i] == d){
           _buckets[index][i] = d;
           return true;
         }
       }
     }
     insert(d);
     return false;
   }

   // return true if inserted successfully (i.e. d is not in the hash)
   // return false is d is already in the hash ==> will not insert
   bool insert(const Data& d) { 
     if(check(d)) return false;
     size_t index = bucketNum(d);
     _buckets[index].push_back(d);
     return true;
   }

   // return true if removed successfully (i.e. d is in the hash)
   // return fasle otherwise (i.e. nothing is removed)
   bool remove(const Data& d) {
     if(check(d)){
       size_t index = bucketNum(d);
       for(size_t i = 0; i < _buckets[index].size(); i++){
         if(_buckets[index][i] == d){
           _buckets[index].erase(_buckets[index].begin()+i);
           return true;
         }
       }
     }
     return false;
   }
private:
   // Do not add any extra data member
   size_t            _numBuckets;
   vector<Data>*     _buckets;

   size_t bucketNum(const Data& d) const {
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H
