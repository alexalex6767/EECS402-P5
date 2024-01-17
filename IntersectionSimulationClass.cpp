#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "IntersectionSimulationClass.h"
#include "random.h"


//Programmer: Andrew Morgan and Tzu-Yu Peng
//Date: November 2023
//Purpose: A class that will act as the basis for an event-driven
//         simulation involving traffic flow through an intersection
//         that is managed via a traffic light.
void IntersectionSimulationClass::readParametersFromFile(
     const string &paramFname){
  bool success = true;;
  ifstream paramF;

  paramF.open(paramFname.c_str());

  //Check that the file was able to be opened...
  if(paramF.fail()){
    success = false;
    cout << "ERROR: Unable to open parameter file: " << paramFname << endl;
  }
  else{
    //Now read in all the params, according to the specified format of
    //the text-based parameter file.
    if(success){
      paramF >> randomSeedVal;
      if(paramF.fail() ||
          randomSeedVal < 0){
        success = false;
        cout << "ERROR: Unable to read/set random generatsor seed" << endl;
      }
    }

    if(success){
      paramF >> timeToStopSim;
      if(paramF.fail() ||
          timeToStopSim <= 0){
        success = false;
        cout << "ERROR: Unable to read/set simulation end time" << endl;
      }
    }

    if(success){
      paramF >> eastWestGreenTime >> eastWestYellowTime;
      if(paramF.fail() ||
          eastWestGreenTime <= 0 ||
          eastWestYellowTime <= 0){
        success = false;
        cout << "ERROR: Unable to read/set east-west times" << endl;
      }
    }

    if(success){
      paramF >> northSouthGreenTime >> northSouthYellowTime;
      if (paramF.fail() ||
          northSouthGreenTime <= 0 ||
          northSouthYellowTime <= 0){
        success = false;
        cout << "ERROR: Unable to read/set north-south times" << endl;
      }
    }

    if(success){
      paramF >> eastArrivalMean >> eastArrivalStdDev;
      if(paramF.fail() ||
          eastArrivalMean <= 0 ||
          eastArrivalStdDev < 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set east arrival distribution" << endl;
      }
    }

    if(success){
      paramF >> westArrivalMean >> westArrivalStdDev;
      if(paramF.fail() ||
          westArrivalMean <= 0 ||
          westArrivalStdDev < 0){
        success = false;
        cout << "ERROR: Unable to read/set west arrival distribution" << endl;
      }
    }

    if(success){
      paramF >> northArrivalMean >> northArrivalStdDev;
      if (paramF.fail() ||
          northArrivalMean <= 0 ||
          northArrivalStdDev < 0){
        success = false;
        cout << "ERROR: Unable to read/set north arrival distribution" << endl;
      }
    }

    if(success){
      paramF >> southArrivalMean >> southArrivalStdDev;
      if (paramF.fail() ||
          southArrivalMean <= 0 ||
          southArrivalStdDev < 0){
        success = false;
        cout << "ERROR: Unable to read/set south arrival distribution" << endl;
      }
    }

    if(success){
      paramF >> percentCarsAdvanceOnYellow;
      if(paramF.fail() ||
          percentCarsAdvanceOnYellow < 0 ||
          percentCarsAdvanceOnYellow > 100){
        success = false;
        cout << "ERROR: Unable to read/set percentage yellow advance" << endl;
      }

      //Use the specified seed to seed the random number generator
      setSeed(randomSeedVal);
    }

    paramF.close();
  }

  //Let the caller know whether things went well or not by printing the
  if(!success){
    cout << "ERROR: Parameter file was NOT read in successfully, so the " <<
            "simulation is NOT setup properly!" << endl;
    isSetupProperly = false;
  }
  else{
    cout << "Parameters read in successfully - simulation is ready!" << endl;
    isSetupProperly = true;
  }
}

void IntersectionSimulationClass::printParameters() const{
  cout << "===== Begin Simulation Parameters =====" << endl;
  if(!isSetupProperly){
    cout << "  Simulation is not yet properly setup!" << endl;
  }
  else{
    cout << "  Random generator seed: " << randomSeedVal << endl;
    cout << "  Simulation end time: " << timeToStopSim << endl;

    cout << "  East-West Timing -" <<
            " Green: " << eastWestGreenTime << 
            " Yellow: " << eastWestYellowTime <<
            " Red: " << getEastWestRedTime() << endl;

    cout << "  North-South Timing -" <<
            " Green: " << northSouthGreenTime << 
            " Yellow: " << northSouthYellowTime <<
            " Red: " << getNorthSouthRedTime() << endl;

    cout << "  Arrival Distributions:" << endl;
    cout << "    East - Mean: " << eastArrivalMean << 
            " StdDev: " << eastArrivalStdDev << endl;
    cout << "    West - Mean: " << westArrivalMean << 
            " StdDev: " << westArrivalStdDev << endl;
    cout << "    North - Mean: " << northArrivalMean << 
            " StdDev: " << northArrivalStdDev << endl;
    cout << "    South - Mean: " << southArrivalMean << 
            " StdDev: " << southArrivalStdDev << endl;

    cout << "  Percentage cars advancing through yellow: " <<
            percentCarsAdvanceOnYellow << endl;
  }
  cout << "===== End Simulation Parameters =====" << endl;
}

// This is the funciton to schedule arrival for cars, Written by me
void IntersectionSimulationClass::scheduleArrival(const string &travelDir){
  int eventType;
  int eventTime;

  if(travelDir == EAST_DIRECTION){
    eventType = EVENT_ARRIVE_EAST;
    eventTime = currentTime + getPositiveNormal(eastArrivalMean,
                                                eastArrivalStdDev);
  }
  else if(travelDir == WEST_DIRECTION){
    eventType = EVENT_ARRIVE_WEST;
    eventTime = currentTime + getPositiveNormal(westArrivalMean,
                                                westArrivalStdDev);
  }
  else if(travelDir == NORTH_DIRECTION){
    eventType = EVENT_ARRIVE_NORTH;
    eventTime = currentTime + getPositiveNormal(northArrivalMean,
                                                 northArrivalStdDev);
  }
  else if(travelDir == SOUTH_DIRECTION){
    eventType = EVENT_ARRIVE_SOUTH;
    eventTime = currentTime + getPositiveNormal(southArrivalMean,
                                                 southArrivalStdDev);
  }
  
  EventClass arrivalEvent(eventTime, eventType);
  eventList.insertValue(arrivalEvent);
  
  cout << "Time: " << currentTime << " Scheduled " << arrivalEvent << endl;
}

// This is the funciton to schedule light change, Written by me
void IntersectionSimulationClass::scheduleLightChange(){
  int eventTime;
  int eventType;
  
  if(currentLight == LIGHT_GREEN_EW){
    eventType = EVENT_CHANGE_YELLOW_EW;
    eventTime = currentTime + eastWestGreenTime;
  }
  else if(currentLight == LIGHT_YELLOW_EW){
    eventType = EVENT_CHANGE_GREEN_NS;
    eventTime = currentTime + eastWestYellowTime;
  }
  else if(currentLight == LIGHT_GREEN_NS){
    eventType = EVENT_CHANGE_YELLOW_NS;
    eventTime = currentTime + northSouthGreenTime;
  }
  else if(currentLight == LIGHT_YELLOW_NS){
    eventType = EVENT_CHANGE_GREEN_EW;
    eventTime = currentTime + northSouthYellowTime;
  }
  
  EventClass arrivalEvent(eventTime, eventType);
  eventList.insertValue(arrivalEvent);
  
  cout << "Time: " << currentTime << " Scheduled " << arrivalEvent << endl;
}

// This is the function for handle next event, Written by me
bool IntersectionSimulationClass::handleNextEvent(){
  EventClass handledEvent;
  eventList.removeFront(handledEvent);
  currentTime = handledEvent.getTimeOccurs();
  
  if(currentTime <= timeToStopSim){
    // do the correct things, otherwise don't do and end
    
    
    // First see Car Events
    // Car from east
    if(handledEvent.getType() == EVENT_ARRIVE_EAST){
      cout << endl;
      cout << "Handling " << handledEvent << endl;
      
      CarClass carEast = CarClass(EAST_DIRECTION, currentTime);
      // put this car in queue
      eastQueue.enqueue(carEast);
      // print that I enqueue it
      cout << "Time: " << currentTime << " Car #" << carEast.getId()
           << " arrives east-bound - queue length: "
           << eastQueue.getNumElems() << endl;
      // Schedule Arrival
      scheduleArrival(EAST_DIRECTION);
      // Change Max Queue for print Statistics
      if(eastQueue.getNumElems() > maxEastQueueLength){
        //count the longest east queue
        maxEastQueueLength = eastQueue.getNumElems();
      }
    }// end of if eventType = east
    
    // Car from West
    else if(handledEvent.getType() == EVENT_ARRIVE_WEST){
      cout << endl;
      cout << "Handling " << handledEvent << endl;
      
      CarClass carWest = CarClass(WEST_DIRECTION, currentTime);
      // put this car in queue
      westQueue.enqueue(carWest);
      // print that I enqueue it
      cout << "Time: " << currentTime << " Car #" << carWest.getId()
           << " arrives west-bound - queue length: "
           << westQueue.getNumElems() << endl;
      // Schedule Arrival
      scheduleArrival(WEST_DIRECTION);
      // Change Max Queue for print Statistics
      if(westQueue.getNumElems() > maxWestQueueLength){
        //count the longest west queue
        maxWestQueueLength = westQueue.getNumElems();
      }
    }// end of if eventType = west
    
    // Car from North
    else if(handledEvent.getType() == EVENT_ARRIVE_NORTH){
      cout << endl;
      cout << "Handling " << handledEvent << endl;
      
      CarClass carNorth = CarClass(NORTH_DIRECTION, currentTime);
      // put this car in queue
      northQueue.enqueue(carNorth);
      // print that I enqueue it
      cout << "Time: " << currentTime << " Car #" << carNorth.getId()
           << " arrives north-bound - queue length: "
           << northQueue.getNumElems() << endl;
      // Schedule Arrival
      scheduleArrival(NORTH_DIRECTION);
      // Change Max Queue for print Statistics
      if(northQueue.getNumElems() > maxNorthQueueLength){
        //count the longest north queue
        maxNorthQueueLength = northQueue.getNumElems();
      }
    }// end of if eventType = north
    
    // Car from South
    else if(handledEvent.getType() == EVENT_ARRIVE_SOUTH){
      cout << endl;
      cout << "Handling " << handledEvent << endl;
      
      CarClass carSouth = CarClass(SOUTH_DIRECTION, currentTime);
      // put this car in queue
      southQueue.enqueue(carSouth);
      // print that I enqueue it
      cout << "Time: " << currentTime << " Car #" << carSouth.getId()
           << " arrives south-bound - queue length: "
           << southQueue.getNumElems() << endl;
      // Schedule Arrival
      scheduleArrival(SOUTH_DIRECTION);
      // Change Max Queue for print Statistics
      if(southQueue.getNumElems() > maxSouthQueueLength){
        //count the longest south queue
        maxSouthQueueLength = southQueue.getNumElems();
      }
    }// end of if eventType = south

    // EW Green to Yellow, needs to print
    // Handling Event Type: Light Change to EW Yellow Time: x
    // Advancing cars on east-west green
    else if(handledEvent.getType() == EVENT_CHANGE_YELLOW_EW){
      cout << endl;
      cout << "Handling " << handledEvent << endl;
      // For count total cars passed during green
      int numAdvancedEast = ZERO;
      int numAdvancedWest = ZERO;
      cout << "Advancing cars on east-west green" << endl;
      
      // East: Queue cannot be 0 and
      // cars to east can't larger than the waiting time
      while((eastQueue.getNumElems() != ZERO) &&
            (numAdvancedEast < eastWestGreenTime)){
        CarClass outCar;
        eastQueue.dequeue(outCar);
        
        cout << "  Car #" << outCar.getId() << " advances east-bound" << endl;
        numAdvancedEast += ONE;
      }
      // West: Queue cannot be 0 and
      // cars to east can't larger than the waiting time
      while((westQueue.getNumElems() != ZERO) &&
            (numAdvancedWest < eastWestGreenTime)){
        CarClass outCar;
        westQueue.dequeue(outCar);
        
        cout << "  Car #" << outCar.getId() << " advances west-bound" << endl;
        numAdvancedWest += ONE;
      }
      
      // print summary
      cout << "East-bound cars advanced on green: " << numAdvancedEast
           << " Remaining queue: " << eastQueue.getNumElems() << endl;
      cout << "West-bound cars advanced on green: " << numAdvancedWest
           << " Remaining queue: " << westQueue.getNumElems() << endl;
      
      //For printing total advanced car at the end
      numTotalAdvancedEast += numAdvancedEast;
      numTotalAdvancedWest += numAdvancedWest;
      
      //Light Change to EW Yellow
      currentLight = LIGHT_YELLOW_EW;
      scheduleLightChange();
    }// end of EW Green to Yellow
    
    // NS Green to Yellow, needs to print
    // Handling Event Type: Light Change to NS Yellow Time: x
    // Advancing cars on north-south green
    else if(handledEvent.getType() == EVENT_CHANGE_YELLOW_NS){
      cout << endl;
      cout << "Handling " << handledEvent << endl;
      // For count total cars passed during green
      int numAdvancedNorth = ZERO;
      int numAdvancedSouth = ZERO;
      cout << "Advancing cars on north-south green" << endl;
      
      // North: Queue cannot be 0 and
      // cars to east can't larger than the waiting time
      while((northQueue.getNumElems() != ZERO) &&
            (numAdvancedNorth < northSouthGreenTime)){
        CarClass outCar;
        northQueue.dequeue(outCar);
        
        cout << "  Car #" << outCar.getId() << " advances north-bound" << endl;
        numAdvancedNorth += ONE;
      }
      // South: Queue cannot be 0 and
      // cars to south can't larger than the waiting time
      while((southQueue.getNumElems() != ZERO) &&
            (numAdvancedSouth < northSouthGreenTime)){
        CarClass outCar;
        southQueue.dequeue(outCar);
        
        cout << "  Car #" << outCar.getId() << " advances south-bound" << endl;
        numAdvancedSouth += ONE;
      }
      
      // print summary
      cout << "North-bound cars advanced on green: " << numAdvancedNorth
           << "Remaining queue: " << northQueue.getNumElems() << endl;
      cout << "South-bound cars advanced on green: " << numAdvancedSouth
           << "Remaining queue: " << southQueue.getNumElems() << endl;
      
      //For printing total advanced car at the end
      numTotalAdvancedNorth += numAdvancedNorth;
      numTotalAdvancedSouth += numAdvancedSouth;
      
      //Light Change to NS Yellow
      currentLight = LIGHT_YELLOW_NS;
      scheduleLightChange();
    }// end of NS Green to Yellow
    
    // EW Yellow to NS Green, needs to print
    // Handling Event Type: Light Change to NS Green Time: x
    // Advancing cars on east-west yellow
    else if(handledEvent.getType() == EVENT_CHANGE_GREEN_NS){
      int numAdvancedEast = ZERO;
      int numAdvancedWest = ZERO;
      bool goOrNotEast = GO_YELLOW;
      bool goOrNotWest = GO_YELLOW;
      
      cout << endl;
      cout << "Handling " << handledEvent << endl;
      
      cout << "Advancing cars on east-west yellow" << endl;
      // East
      while((eastQueue.getNumElems() != ZERO) &&
            (numAdvancedEast < eastWestYellowTime) &&
            (goOrNotEast == GO_YELLOW)){
        int passNumber = getUniform(ONE, ONE_HUNDRED);
        if(passNumber > percentCarsAdvanceOnYellow){
          cout << " Next east-bound car will NOT advance on yellow" << endl;
          goOrNotEast = STOP_YELLOW;
        }
        else{
          CarClass outCar;
          eastQueue.dequeue(outCar);
          cout << " Next east-bound car will NOT advance on yellow" << endl;
          cout << "  Car #" << outCar.getId()
               << " advances east-bound" << endl;
          numAdvancedEast += ONE;
        }
      }
      if(eastQueue.getNumElems() == ZERO){
        cout << "  No east-bound cars waiting to advance on yellow" << endl;
      }
      
      //West
      while((westQueue.getNumElems() != ZERO) &&
            (numAdvancedWest < eastWestYellowTime) &&
            (goOrNotWest == GO_YELLOW)){
        int passNumber = getUniform(ONE, ONE_HUNDRED);
        if(passNumber > percentCarsAdvanceOnYellow){
          cout << " Next west-bound car will NOT advance on yellow" << endl;
          goOrNotWest = STOP_YELLOW;
        }
        else{
          CarClass outCar;
          westQueue.dequeue(outCar);
          cout << " Next west-bound car will NOT advance on yellow" << endl;
          cout << "  Car #" << outCar.getId()
               << " advances west-bound" << endl;
          numAdvancedWest += ONE;
        }
      }
      if(westQueue.getNumElems() == ZERO){
        cout << "  No west-bound cars waiting to advance on yellow" << endl;
      }
      
      // print summary
      cout << "East-bound cars advanced on yellow: " << numAdvancedEast
           << "Remaining queue: " << eastQueue.getNumElems() << endl;
      cout << "West-bound cars advanced on yellow: " << numAdvancedWest
           << "Remaining queue: " << westQueue.getNumElems() << endl;
      
      //For printing total advanced car at the end
      numTotalAdvancedEast += numAdvancedEast;
      numTotalAdvancedWest += numAdvancedWest;
      
      //Light Change to NS Green
      currentLight = LIGHT_GREEN_NS;
      scheduleLightChange();
      
      
    }// end of EW Yellow to NS Green
      
    // NS Yellow to EW Green, needs to print
    // Handling Event Type: Light Change to EW Green Time: x
    // Advancing cars on north-south yellow
    else if(handledEvent.getType() == EVENT_CHANGE_GREEN_EW){
      int numAdvancedNorth = ZERO;
      int numAdvancedSouth = ZERO;
      bool goOrNotNorth = GO_YELLOW;
      bool goOrNotSouth = GO_YELLOW;
      
      cout << endl;
      cout << "Handling " << handledEvent << endl;
      
      cout << "Advancing cars on north-south yellow" << endl;
      // North
      while((northQueue.getNumElems() != ZERO) &&
            (numAdvancedNorth < northSouthYellowTime) &&
            (goOrNotNorth == GO_YELLOW)){
        int passNumber = getUniform(ONE, ONE_HUNDRED);
        if(passNumber > percentCarsAdvanceOnYellow){
          cout << " Next north-bound car will NOT advance on yellow" << endl;
          goOrNotNorth = STOP_YELLOW;
        }
        else{
          CarClass outCar;
          northQueue.dequeue(outCar);
          cout << " Next north-bound car will NOT advance on yellow" << endl;
          cout << "  Car #" << outCar.getId()
               << " advances north-bound" << endl;
          numAdvancedNorth += ONE;
        }
      }
      if(northQueue.getNumElems() == ZERO){
        cout << "  No north-bound cars waiting to advance on yellow" << endl;
      }
      
      //South
      while((southQueue.getNumElems() != ZERO) &&
            (numAdvancedSouth < northSouthYellowTime) &&
            (goOrNotSouth == GO_YELLOW)){
        int passNumber = getUniform(ONE, ONE_HUNDRED);
        if(passNumber > percentCarsAdvanceOnYellow){
          cout << " Next south-bound car will NOT advance on yellow" << endl;
          goOrNotSouth = STOP_YELLOW;
        }
        else{
          CarClass outCar;
          southQueue.dequeue(outCar);
          cout << " Next south-bound car will NOT advance on yellow" << endl;
          cout << "  Car #" << outCar.getId()
               << " advances south-bound" << endl;
          numAdvancedSouth += ONE;
        }
      }
      if(southQueue.getNumElems() == ZERO){
        cout << "  No south-bound cars waiting to advance on yellow" << endl;
      }
      
      // print summary
      cout << "North-bound cars advanced on yellow: " << numAdvancedNorth
           << "Remaining queue: " << northQueue.getNumElems() << endl;
      cout << "South-bound cars advanced on yellow: " << numAdvancedSouth
           << "Remaining queue: " << southQueue.getNumElems() << endl;
      
      //For printing total advanced car at the end
      numTotalAdvancedNorth += numAdvancedNorth;
      numTotalAdvancedSouth += numAdvancedSouth;
      
      //Light Change to NS Yellow
      currentLight = LIGHT_GREEN_EW;
      scheduleLightChange();
      
      
    }// end of NS Yellow to EW Green

    return true;
  }// end of if within the time to stop sim
  // if over simulation time
  cout << endl;
  cout << "Next event occurs AFTER the simulation end time "
       << "(" << handledEvent << ")!" << endl;
  
  return false;
}

void IntersectionSimulationClass::printStatistics() const{
  cout << "===== Begin Simulation Statistics =====" << endl;
  cout << "  Longest east-bound queue: " << maxEastQueueLength << endl;
  cout << "  Longest west-bound queue: " << maxWestQueueLength << endl;
  cout << "  Longest north-bound queue: " << maxNorthQueueLength << endl;
  cout << "  Longest south-bound queue: " << maxSouthQueueLength << endl;
  cout << "  Total cars advanced east-bound: " <<
          numTotalAdvancedEast << endl;
  cout << "  Total cars advanced west-bound: " <<
          numTotalAdvancedWest << endl;
  cout << "  Total cars advanced north-bound: " <<
          numTotalAdvancedNorth << endl;
  cout << "  Total cars advanced south-bound: " <<
          numTotalAdvancedSouth << endl;
  cout << "===== End Simulation Statistics =====" << endl;
}
