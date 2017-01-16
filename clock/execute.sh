#!/bin/bash

python getdate.py > temp
echo "clock_realtime.out temp -1" | ./proc.out
