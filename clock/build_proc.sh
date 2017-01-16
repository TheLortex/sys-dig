#!/bin/bash
cpp -P ../mjblocks/proco.mmj > proc.mj
../minijazz/mjc.native proc.mj
../simulateur_netlist/compilateur_netlist proc.net proc.out "  |hour0||hour1|:|min0||min1|:|sec0||sec1|\n\n|day0||day1|/|month0||month1|/|year0||year1||year2||year3|"

