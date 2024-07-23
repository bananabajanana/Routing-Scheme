#CC = /loc/packages/gcc/2.95.3/sunos-5.7/bin/g++
CC = /loc/packages/gcc/2.95.3/Linux-2.4.18/bin/g++
LD = g++
#INC = -I$(LEDALib)/incl
CFLAGS = -c -O2 -Wl,--wrap,__ctype_b

default: stuff

stuff: ../ctype_fixer.o ../rand/mersenne.o ../util.o multiEdge.o multiNode.o multiGraph.o multiGraphManager.o ../generators/LuTopologyGenerator.o xmltester.o
	$(CC) -O2 -Wl,--wrap,__ctype_b ../ctype_fixer.o ../rand/mersenne.o ../util.o multiEdge.o multiNode.o multiGraph.o multiGraphManager.o ../generators/LuTopologyGenerator.o xmltester.o -o stuff

xmltester.o: xmltester.cc
	$(CC) $(CFLAGS) xmltester.cc

../geneators/LuTopologyGenerator.o: ../generators/LuTopologyGenerator.cc
	$(CC) $(CFLAGS) -o ../generators/LuTopologyGenerator.o ../generators/LuTopologyGenerator.cc

multiGraphManager.o: multiGraphManager.cc
	$(CC) $(CFLAGS) multiGraphManager.cc

multiGraph.o: multiGraph.cc
	$(CC) $(CFLAGS) multiGraph.cc

multiNode.o: multiNode.cc
	$(CC) $(CFLAGS) multiNode.cc

multiEdge.o: multiEdge.cc
	$(CC) $(CFLAGS) multiEdge.cc

../util.o: ../util.cc
	$(CC) $(CFLAGS) -o ../util.o ../util.cc

../rand/mersenne.o: ../rand/mersenne.cpp
	$(CC) $(CFLAGS) -o ../rand/mersenne.o ../rand/mersenne.cpp

clean:
	-rm *.o *.xml stuff core
