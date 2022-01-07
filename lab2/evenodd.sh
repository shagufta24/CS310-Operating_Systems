#!/bin/bash

#Even or Odd
echo "Enter a number: "
read num

if [ $(($num % 2)) -eq 0 ] #Comparing with b
then
echo Even number
else
echo Odd number
fi