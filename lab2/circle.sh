#!/bin/bash

#Circle area, circumference
echo -n "Enter circle radius: "
read rad
circumference=$(echo "2*3.141*$rad"|bc)
area=$(echo "3.14*$rad*$rad"|bc)
echo "Area: $area, Circumference: $circumference" $'\n'


