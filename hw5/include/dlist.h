/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { 
        _node = _node->_next;
        return *(this); 
      }
      iterator operator ++ (int) {
        iterator temp(_node);
        _node = _node->_next;
        return temp;
      }
      iterator& operator -- () {
        _node = _node->_prev;
        return *(this);
      }
      iterator operator -- (int) {
        iterator temp(_node);
        _node = _node->_prev;
        return temp;
      }

      iterator& operator = (const iterator& i) { 
        _node = i._node;
        return *(this); 
      }

      bool operator != (const iterator& i) const {
        if(_node != i._node) return true;
        else return false;
      }
      bool operator == (const iterator& i) const {
        if(_node == i._node) return true;
        else return false;
      }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const {
     if(_head->_next == _head) return iterator(_head);
     else return iterator(_head->_next);
   }
   iterator end() const { return iterator(_head); }
   bool empty() const {
     if(begin() == end()) return true;
     else return false;
   }

   size_t size() const {
     if(empty()) return 0;
     else{
       int count = 0;
       for(iterator i = begin(); i != end(); i++)
         count++;
       return count;
     }
   }

   void push_back(const T& x) {
     DListNode<T>* newNode = new DListNode<T>(x, _head->_prev, _head);
     (_head->_prev)->_next = newNode;
     _head->_prev = newNode;
    }

   void pop_front() {
     if(!empty()){
       DListNode<T>* Delete = _head->_next;
       (Delete->_next)->_prev = _head;
       _head->_next = Delete->_next;
       delete Delete;
     }
    }
   void pop_back() {
     if(!empty()){
       DListNode<T>* Delete = _head->_prev;
       (Delete->_prev)->_next = _head;
       _head->_prev = Delete->_prev;
       delete Delete;
     }
    }

   // return false if nothing to erase
   bool erase(iterator pos) {
     if(empty()) return false;
     else{
      ((pos._node)->_prev)->_next = (pos._node)->_next;
      ((pos._node)->_next)->_prev = (pos._node)->_prev;
      delete pos._node;
      return true;
     }
   }
   bool erase(const T& x) {
     for(iterator i = begin(); i != end(); i++){
       if(*i == x){
         ((i._node)->_prev)->_next = (i._node)->_next;
         ((i._node)->_next)->_prev = (i._node)->_prev;
         delete i._node;
         return true;
       }
     }
     return false;
   }

   void clear() {
     iterator d(_head->_next);
     while(erase(d)){
       d = (_head->_next);
     }
   }  // delete all nodes except for the dummy node

   void sort() const {
     iterator i1 (_head->_next);
     iterator i2 ((i1._node)->_next); 
     for(int i = 0; i < size(); i++){
       if(*i1 > *i2){
         T temp = *i1;
         *i1 = *i2;
         *i2 = temp;
       }
       i1++;
       i2++;
     }
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
