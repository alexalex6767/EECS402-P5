all: proj5.exe
 
proj5.o: project5.cpp IntersectionSimulationClass.h
	g++ -std=c++98 -g -Wall -c project5.cpp -o proj5.o
 
CarClass.o: CarClass.cpp CarClass.h constants.h
	g++ -std=c++98 -g -Wall -c CarClass.cpp -o CarClass.o
  
EventClass.o: EventClass.cpp EventClass.h constants.h
	g++ -std=c++98 -g -Wall -c EventClass.cpp -o EventClass.o
 
IntersectionSimulationClass.o: IntersectionSimulationClass.cpp IntersectionSimulationClass.h CarClass.h EventClass.h SortedListClass.h SortedListClass.inl FIFOQueueClass.h FIFOQueueClass.inl 
	g++ -std=c++98 -g -Wall -c IntersectionSimulationClass.cpp -o IntersectionSimulationClass.o
 
random.o: random.cpp random.h
	g++ -std=c++98 -g -Wall -c random.cpp -o random.o

proj5.exe: CarClass.o EventClass.o IntersectionSimulationClass.o random.o proj5.o
	g++ -std=c++98 CarClass.o EventClass.o IntersectionSimulationClass.o random.o proj5.o -o proj5.exe
 
clean:
	rm -f *.o *.exe

