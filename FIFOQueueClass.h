#ifndef FIFOQueueClass_h
#define FIFOQueueClass_h
#include "LinkedNodeClass.h"
//FIFOQueueClass.h, project4
//Programmer: Tzu-Yu Peng, typeng
//Date: Nov 9th 2023
//This class is for making a Queue Data Structure.

//Note:: this class does not have copy ctor ot operator assignment
//using these will be a shallow copy

template<class T>
class FIFOQueueClass
{
  private:
    //Points to the first node in a queue, or NULL
    //if queue is empty.
    LinkedNodeClass<T> *head;
    //Points to the last node in a queue, or NULL
    //if queue is empty.
    LinkedNodeClass<T> *tail;
  
  public:
    //Default Constructor. Initialize to empty
    FIFOQueueClass();
    //NOTE: This class does NOT have a copy ctor or an overloaded
    // assignment operator – therefore, using either of those
    // things will result in a shallow copy. Users should not
    // attempt to copy a FIFOQueueClass object using either of
    // these approaches!
  
    //Destructor. Responsible for making sure any dynamic memory
    //associated with an object is freed up when the object is
    //being destroyed.
    ~FIFOQueueClass();
  
    //Inserts the value provided (newItem) into the queue.
    void enqueue(const T &newItem);
  
    //Attempts to take the next item out of the queue. If the
    //queue is empty, the function returns false and the state
    //of the reference parameter (outItem) is undefined. If the
    //queue is not empty, the function returns true and outItem
    //becomes a copy of the next item in the queue, which is
    //removed from the data structure.
    bool dequeue(T &outItem);
  
    //Prints out the contents of the queue. All printing is done
    //on one line, using a single space to separate values, and a
    //single newline character is printed at the end. Values will
    //be printed such that the next value that would be dequeued
    //is printed first.
    void print() const;
  
    //Returns the number of nodes contained in the queue.
    int getNumElems() const;
  
    //Clears the queue to an empty state without resulting in any
    //memory leaks.
    void clear();
};

#include "FIFOQueueClass.inl"
#endif /* FIFOQueueClass_h */
