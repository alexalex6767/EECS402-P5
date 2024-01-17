#ifndef LinkedNodeClass_h
#define LinkedNodeClass_h

#include <iostream>
using namespace std;
//LinkedNodeClass.h, project4
//Programmer: Tzu-Yu Peng, typeng
//Date: Nov 9th 2023

//The list node class will be the data type for individual nodes of
//a doubly-linked data structure.
template<class T>
class LinkedNodeClass
{
  private:
  //This is a pointer that points to previous data
  LinkedNodeClass<T> *prevNode;

  //The value contained within this node.
  T nodeVal;

  //This is a pointer that points to next data
  LinkedNodeClass<T> *nextNode;
  
public:
  //The ONLY constructor for the linked node class - it takes in the
  //newly created node's previous pointer, value, and next pointer,
  //and assigns them.]
  //Param1: Address of node that comes before this one,
  //Param2: Value to be contained in this node,
  //Param3: Address of node that comes after this one
  LinkedNodeClass(LinkedNodeClass<T> *inPrev,
                  const T &inVal,
                  LinkedNodeClass<T> *inNext);

  //Returns the value stored within this node.
  T getValue() const;

  //Returns the address of the node that follows this node.
  LinkedNodeClass<T>* getNext() const;

  //Returns the address of the node that comes before this node.
  LinkedNodeClass<T>* getPrev() const;

  //Sets the object’s next node pointer to NULL.
  void setNextPointerToNull();

  //Sets the object's previous node pointer to NULL.
  void setPreviousPointerToNull();

  //This function DOES NOT modify "this" node. Instead, it uses
  //the pointers contained within this node to change the previous
  //and next nodes so that they point to this node appropriately.
  //In other words, if "this" node is set up such that its prevNode
  //pointer points to a node (call it "A"), and "this" node's
  //nextNode pointer points to a node (call it "B"), then calling
  //setBeforeAndAfterPointers results in the node we're calling
  //"A" to be updated so its "nextNode" points to "this" node, and
  //the node we're calling "B" is updated so its "prevNode" points
  //to "this" node, but "this" node itself remains unchanged.
  void setBeforeAndAfterPointers();
};
#include "LinkedNodeClass.inl"
#endif /* LinkedNodeClass_h */
