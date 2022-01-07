#!/bin/bash

#Adding numbers
echo "Adding 2 numbers"$'\n'
echo -n "Enter number 1: "
read num1
echo -n "Enter number 2: "
read num2
sum=$(($num1 + $num2))
echo The sum is: $sum
