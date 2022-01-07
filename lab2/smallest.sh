#!/bin/bash

#Smallest of 3 numbers
echo Enter 3 numbers with spaces inbetween
read a b c

s=$a #Smallest set to a

if [ $b -lt $s ] #Comparing with b
then
s=$b
fi

if [ $c -lt $s ] #Comapring with c
then
s=$c
fi

echo Smallest of $a, $b, $c is $s