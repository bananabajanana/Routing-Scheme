CC = /usr/bin/g++-4.8
CXXFLAGS = -O2 -Wno-deprecated

default: generator

generator: ../ctype_fixer.o ../rand/mersenne.o ../util.o ../multi/multiEdge.o ../multi/multiNode.o ../multi/multiGraph.o ../multi/multiGraphManager.o LuTopologyGenerator.o generator.o
	$(CC) $(CXXFLAGS) ../ctype_fixer.o ../rand/mersenne.o ../util.o ../multi/multiEdge.o ../multi/multiNode.o ../multi/multiGraph.o ../multi/multiGraphManager.o LuTopologyGenerator.o generator.o -o generate

analyzer: ../rand/mersenne.o ../util.o ../multi/multiEdge.o ../multi/multiNode.o ../multi/multiGraph.o ../multi/multiGraphManager.o LuTopologyGenerator.o analyzer.o
	$(CC) $(CXXFLAGS) ../ctype_fixer.o ../rand/mersenne.o ../util.o ../multi/multiEdge.o ../multi/multiNode.o ../multi/multiGraph.o ../multi/multiGraphManager.o LuTopologyGenerator.o analyzer.o -o analyze

analyzer.o: analyzer.cc
	$(CC) -c $(CXXFLAGS) analyzer.cc

generator.o: generator.cc
	$(CC) -c $(CXXFLAGS) generator.cc

LuTopologyGenerator.o: LuTopologyGenerator.cc
	$(CC) -c $(CXXFLAGS) LuTopologyGenerator.cc

../multi/multiGraphManager.o: ../multi/multiGraphManager.cc
	$(CC) -c $(CXXFLAGS) ../multi/multiGraphManager.cc -o ../multi/multiGraphManager.o

../multi/multiGraph.o: ../multi/multiGraph.cc
	$(CC) -c $(CXXFLAGS) ../multi/multiGraph.cc -o ../multi/multiGraph.o

../multi/multiNode.o: ../multi/multiNode.cc
	$(CC) -c $(CXXFLAGS) ../multi/multiNode.cc -o ../multi/multiNode.o

../multi/multiEdge.o: ../multi/multiEdge.cc
	$(CC) -c $(CXXFLAGS) ../multi/multiEdge.cc -o ../multi/multiEdge.o

../util.o: ../util.cc
	$(CC) -c $(CXXFLAGS) ../util.cc -o ../util.o

../rand/mersenne.o: ../rand/mersenne.cpp
	$(CC) -c $(CXXFLAGS) ../rand/mersenne.cpp -o ../rand/mersenne.o

clean:
	-rm *.o *.xml *.copy stuff core
