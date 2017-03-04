Fit : Fit.o FitRoutine.o sort.o
	g++ -o Fit Fit.o FitRoutine.o sort.o

Fit.o : Fit.cpp
	g++ -c Fit.cpp

FitRoutine.o : FitRoutine.cpp
	g++ -c FitRoutine.cpp

	
sort.o : sort.cpp
	g++ -c sort.cpp
	
clean : 
	rm -f Fit.o FitRoutine.o sort.o Fit
