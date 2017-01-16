#!/bin/bash
cpp -P ../mjblocks/proco.mmj > proc.mj
../minijazz/mjc.native proc.mj
../simulateur_netlist/compilateur_netlist proc.net proc.out "|sec0||sec1||min0||min1||hour0||hour1|\n|day0||day1||month0||month1|"
