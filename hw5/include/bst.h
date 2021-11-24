/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
   friend class BSTree<T>;
   friend class BSTree<T>::iterator;

   BSTreeNode(const T& d, BSTreeNode<T>* p = 0, BSTreeNode<T>* l = 0, BSTreeNode<T>* r = 0):
   _data(d),_parent(p),_left(l),_right(r) {}

   T               _data;
   BSTreeNode<T>*  _parent;
   BSTreeNode<T>*  _left;
   BSTreeNode<T>*  _right;
};


template <class T>
class BSTree
{
   // TODO: design your own class!!
public:
   BSTree(){
     _root = new BSTreeNode<T>(T());
   }
   ~BSTree(){clear(); delete _root;}

   class iterator {
     friend class BSTree<T>;
   public:
      iterator(BSTreeNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator(){}

      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () {
        BSTreeNode<T>* root = _node;
        while(root->_parent != NULL)
           root = root->_parent;
        if(_node == rightmost(root)){
          _node = leftmost(root);
        }
        else{
          if(_node->_right == NULL){
            if((_node->_parent)->_left == _node){
              _node = _node->_parent;
            }
            else{
              while((_node->_parent)->_left != _node)
                _node = _node->_parent;
            }
          }
          else{
            _node = leftmost(_node->_right);
          }
        }
        return *(this); 
      }

      iterator operator ++ (int) {
        iterator temp(_node);
        ++(*this);
        return temp;
      }

      iterator& operator -- () {
        BSTreeNode<T>* root = _node;
        while(root->_parent != NULL)
           root = root->_parent;
       if(_node == leftmost(root)){
          _node = rightmost(root);
        }
        else{
          if(_node->_left == NULL){
            if((_node->_parent)->_right == _node){
              _node = _node->_parent;
            }
            else{
              while((_node->_parent)->_right != _node)
                _node = _node->_parent;
            }
          }
          else{
            _node = rightmost(_node->_left);
          }
        }
        return *(this); 
      }

      iterator operator -- (int) {
        iterator temp(_node);
        --(*this);
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
      BSTreeNode<T>* _node;
      BSTreeNode<T>* leftmost(BSTreeNode<T>* n){
     while(n->_left != NULL){
       n = n->_left;
     }
     return n;
     }

     BSTreeNode<T>* rightmost(BSTreeNode<T>* n){
     while(n->_right != NULL){
       n = n->_right;
     }
     return n;
   }
    };

    iterator begin() const{
      if(_size == 0) return NULL;
      else return iterator(leftmost(_root));
    }
    iterator end() const{
      if(_size == 0) return NULL;
      else return iterator(rightmost(_root));
    }
    bool empty() const{
      if(_size == 0) return true;
      else return false;
    }

    size_t size() const{return _size;}

    void insert(const T& x){
      if(_size == 0){
        _root->_data = x;
        _size++;
      }
      else{
        insert(x,_root);
        _size++;
      }

    }
    void pop_front(){
      if(!empty()){
        if(_size == 1) _size = 0;
        else {
          BSTreeNode<T>* d = leftmost(_root);
          if(d = _root){
            _root = _root->_right;
            _root->_parent = NULL;
            delete d;
            _size--;
          }
          else{
            if(d->_right == NULL){
              (d->_parent)->_left = NULL;
              delete d;
              _size--;
            }
            else{
              (d->_parent)->_left = d->_right;
              (d->_right)->_parent = d->_parent;
              delete d;
              _size--;
            }
          }
        }
      }
    }

    void pop_back(){
      if(!empty()){
        if(_size == 1) _size = 0;
        else {
          BSTreeNode<T>* d = rightmost(_root);
          if(d = _root){
            _root = _root->_left;
            _root->_parent = NULL;
            delete d;
            _size--;
          }
          else{
            if(d->_left == NULL){
              (d->_parent)->_right = NULL;
              delete d;
              _size--;
            }
            else{
              (d->_parent)->_right = d->_left;
              (d->_left)->_parent = d->_parent;
              delete d;
              _size--;
            }
          }
        }
      }      
    }
    bool erase(iterator pos){
      if(empty()) return false;
      else return erase(*pos);
    }
    bool erase(const T& x){
      BSTreeNode<T>* n = search(x,_root);
      if(n == NULL) return false;
      else{
        if(n = _root){
          if(_size = 1) _size--;
          else{
            if(_root->_right != NULL && _root->_left == NULL){
              BSTreeNode<T>* temp = _root->_right;
              delete _root;
              _root = temp;
              _root->_parent = NULL;
            }
            else if(_root->_right == NULL && _root->_left != NULL){
              BSTreeNode<T>* temp = _root->_left;
              delete _root;
              _root = temp;
              _root->_parent = NULL;
            }
            else{
              BSTreeNode<T>* suc = leftmost(_root->_right);
              if(suc = _root->_right){
                BSTreeNode<T>* temp = _root->_right;
                temp->_left = _root->_left;
                delete _root;
                _root = temp;
                _root->_parent = NULL;
              }
              else{
                _root->_data = suc->_data;
                if(suc->_right != NULL){
                  (suc->_parent)->_left = suc->_right;
                  (suc->_right)->_parent = suc->_parent;
                }
                delete suc;
              }
            }
            _size--;
            }
        }


        else if((n->_parent)->_right == n){
          if(n->_right == NULL && n->_left == NULL){
            (n->_parent)->_right = NULL;
            delete n;
          }
          else if(n->_right != NULL && n->_left == NULL){
            (n->_parent)->_right = n->_right;
            (n->_right)->_parent = n->_parent;
            delete n;
          }
          else if(n->_right == NULL && n->_left != NULL){
            (n->_parent)->_right = n->_left;
            (n->_left)->_parent = n->_parent;
            delete n;
          }
          else{
            BSTreeNode<T>* suc = leftmost(n->_right);
            if(suc = n->_right){
              (n->_parent)->_right = suc;
              suc->_parent = n->_parent;
              suc->_left = n->_left;
              delete n;
            }
            else{
              n->_data = suc->_data;
              if(suc->_right != NULL){
                (suc->_parent)->_left = suc->_right;
                (suc->_right)->_parent = suc->_parent;
              }
              delete suc;
            }
          }
          _size--;
        }

        else if((n->_parent)->_left == n){
          if(n->_right == NULL && n->_left == NULL){
            (n->_parent)->_left = NULL;
            delete n;
          }
          else if(n->_right != NULL && n->_left == NULL){
            (n->_parent)->_left = n->_right;
            (n->_right)->_parent = n->_parent;
            delete n;
          }
          else if(n->_right == NULL && n->_left != NULL){
            (n->_parent)->_left = n->_left;
            (n->_left)->_parent = n->_parent;
            delete n;
          }
          else{
            BSTreeNode<T>* suc = leftmost(n->_right);
            if(suc = n->_right){
              (n->_parent)->_left = suc;
              suc->_parent = n->_parent;
              suc->_left = n->_left;
              delete n;
            }
            else{
              n->_data = suc->_data;
              if(suc->_right != NULL){
                (suc->_parent)->_left = suc->_right;
                (suc->_right)->_parent = suc->_parent;
              }
              delete suc;
            }
          }
          _size--;
        }        
      }
      return true;
    }
    void clear(){
      if(!empty()){
        if(_root->_left != NULL){
          clear(_root->_left);
        }

        if(_root->_right != NULL){
          clear(_root->_right);
        }
        _root->_left = NULL;
        _root->_right = NULL;
        _size = 0;
      }
    }
    BSTreeNode<T>* Root(){return _root;}

    void print()const{}
    void sort(){}

   private:
   BSTreeNode<T>*  _root;
   size_t _size;

   BSTreeNode<T>* search(const T& x, BSTreeNode<T>* n){
     if(n == NULL) return NULL;
     if(n->_data == x) return n;
     else if(n->_data < x) return search(x,n->_right);
     else  return search(x,n->_left);
   }

   void clear(BSTreeNode<T>* n){
     if(n->_left != NULL)
       clear(n->_left);
     if(n->_right != NULL)
       clear(n->_right);
     delete n;

     }
     
   void insert(const T& x, BSTreeNode<T>* n){
     if(x > n->_data){
       if(n->_right == NULL){
         n->_right = new BSTreeNode<T>(T(x),n);
       }
       else insert(x,n->_right);
     }
     else{
       if(n->_left == NULL){
         n->_left = new BSTreeNode<T>(T(x),n);
       }
       else insert(x,n->_left);
     }
   }

   BSTreeNode<T>* leftmost(BSTreeNode<T>* n) const{
     while(n->_left != NULL){
       n = n->_left;
     }
     return n;
   }

   BSTreeNode<T>* rightmost(BSTreeNode<T>* n) const{
     while(n->_right != NULL){
       n = n->_right;
     }
     return n;
   }

};

#endif // BST_H
