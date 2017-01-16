#!/bin/bash

cd minijazz
ocamlbuild -use-menhir mjc.native
cd ../as
make all
cd ../simulateur_netlist
make all
cd ..
cd clock
./build_proc.sh
