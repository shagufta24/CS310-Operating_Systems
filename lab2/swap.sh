#!/bin/bash

#Swap numbers
echo "Swapping numbers" $'\n'
echo -n "Enter 1st number: "
read n1
echo -n "Enter 2nd number: "
read n2
temp=$n1
n1=$n2
n2=$temp
echo "Swapped numbers: $n1 , $n2"