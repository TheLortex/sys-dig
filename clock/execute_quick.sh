#!/bin/bash

../as/as.native -b program.arm
echo "clock_quick.out ~ -1" | ./proc.out
