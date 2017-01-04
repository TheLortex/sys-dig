CC = g++
FLAGS = -std=c++11 -Wall -W
ODIR = obj
EXEC = compilateur_netlist
PDIR = src/parser
TDIR = src/types

TYPESHEADER = $(wildcard $(TDIR)/*.hpp)

all: $(EXEC)

$(ODIR)/Parser.o: $(PDIR)/Parser.cpp $(PDIR)/Parser.hpp $(TYPESHEADER)
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/Expression.o: $(TDIR)/Expression.cpp $(TDIR)/Expression.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/ExpressionBinary.o: $(TDIR)/ExpressionBinary.cpp $(TDIR)/ExpressionBinary.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/ExpressionUnary.o: $(TDIR)/ExpressionUnary.cpp $(TDIR)/ExpressionUnary.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/ExpressionSelect.o: $(TDIR)/ExpressionSelect.cpp $(TDIR)/ExpressionSelect.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/ExpressionSlice.o: $(TDIR)/ExpressionSlice.cpp $(TDIR)/ExpressionSlice.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/ExpressionReg.o: $(TDIR)/ExpressionReg.cpp $(TDIR)/ExpressionReg.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/ExpressionRam.o: $(TDIR)/ExpressionRam.cpp $(TDIR)/ExpressionRam.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/ExpressionRom.o: $(TDIR)/ExpressionRom.cpp $(TDIR)/ExpressionRom.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/ExpressionMux.o: $(TDIR)/ExpressionMux.cpp $(TDIR)/ExpressionMux.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/Var.o: $(TDIR)/Var.cpp $(TDIR)/Var.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/VarConst.o: $(TDIR)/VarConst.cpp $(TDIR)/VarConst.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/Program.o: src/Program.cpp src/Program.hpp $(TDIR)/Var.hpp $(TDIR)/Expression.hpp $(PDIR)/Parser.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/Graph.o: src/Graph.cpp src/Graph.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(ODIR)/main.o: src/main.cpp $(PDIR)/Parser.hpp src/Program.hpp
	$(CC) -o $@ -c $< $(FLAGS)

$(TDIR)/Expression.hpp: $(TDIR)/Var.hpp
$(TDIR)/ExpressionBinary.hpp: $(TDIR)/Expression.hpp
$(TDIR)/ExpressionUnary.hpp: $(TDIR)/Expression.hpp
$(TDIR)/ExpressionSelect.hpp: $(TDIR)/Expression.hpp
$(TDIR)/ExpressionSlice.hpp: $(TDIR)/Expression.hpp
$(TDIR)/ExpressionReg.hpp: $(TDIR)/Expression.hpp
$(TDIR)/ExpressionRom.hpp: $(TDIR)/Expression.hpp
$(TDIR)/ExpressionRam.hpp: $(TDIR)/Expression.hpp
$(TDIR)/ExpressionMux.hpp: $(TDIR)/Expression.hpp

$(TDIR)/VarConst.hpp: $(TDIR)/Var.hpp

$(EXEC): $(ODIR)/main.o $(ODIR)/Parser.o $(ODIR)/Expression.o $(ODIR)/ExpressionBinary.o $(ODIR)/ExpressionUnary.o $(ODIR)/ExpressionSelect.o $(ODIR)/ExpressionSlice.o $(ODIR)/ExpressionReg.o $(ODIR)/ExpressionRam.o $(ODIR)/ExpressionRom.o $(ODIR)/ExpressionMux.o $(ODIR)/Var.o $(ODIR)/VarConst.o $(ODIR)/Program.o $(ODIR)/Graph.o
	$(CC) -o $@ $^ $(FLAGS)


.PHONY: clean

clean:
	rm -rf $(ODIR)/*.o;
	rm -rf test


