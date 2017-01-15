#!/bin/bash
cpp -P ../mjblocks/proco.mmj > proc.mj
../minijazz/mjc.native proc.mj
../simulateur_netlist/compilateur_netlist proc.net proc.out "|sec0||sec1|"
