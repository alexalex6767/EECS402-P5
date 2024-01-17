#include <iostream>
using namespace std;

#include "EventClass.h"
#include "constants.h"
//Programmer: Andrew Morgan and Tzu-Yu Peng
//Date: November 2020
//Purpose: A simple class to represent event objects that can be used in
//         an event-driven simulation.  The overloaded insertion operator
//         for this class ties it to a simulation of traffic flow through
//         an intersection managed by a traffic light, but the rest of
//         the class is intended to be more generic to represent events
//         in most event-driven simulations.
// I added two overloaded operator for comparing the first and later
// of the events

ostream& operator<<(ostream& outStream,
                    const EventClass &eventToPrint){
  outStream << "Event Type: ";
  if(eventToPrint.typeId == EVENT_ARRIVE_EAST){
    outStream << "East-Bound Arrival";
  }
  else if(eventToPrint.typeId == EVENT_ARRIVE_WEST){
    outStream << "West-Bound Arrival";
  }
  else if(eventToPrint.typeId == EVENT_ARRIVE_NORTH){
    outStream << "North-Bound Arrival";
  }
  else if(eventToPrint.typeId == EVENT_ARRIVE_SOUTH){
    outStream << "South-Bound Arrival";
  }
  else if(eventToPrint.typeId == EVENT_CHANGE_GREEN_EW){
    outStream << "Light Change to EW Green";
  }
  else if(eventToPrint.typeId == EVENT_CHANGE_YELLOW_EW){
    outStream << "Light Change to EW Yellow";
  }
  else if(eventToPrint.typeId == EVENT_CHANGE_GREEN_NS){
    outStream << "Light Change to NS Green";
  }
  else if(eventToPrint.typeId == EVENT_CHANGE_YELLOW_NS){
    outStream << "Light Change to NS Yellow";
  }
  else{
    outStream << "UNKNOWN";
  }
  outStream << " Time: " << eventToPrint.timeOccurs;
  return outStream;
}

// These two are the overloaded operators tow compare the time of events
bool EventClass::operator<=(const EventClass& rhs){
  return timeOccurs <= rhs.timeOccurs;
}

void EventClass::operator=(const EventClass &rhs){
  timeOccurs = rhs.timeOccurs;
  typeId = rhs.typeId;
}
