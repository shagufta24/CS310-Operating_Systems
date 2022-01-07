#!/bin/bash
echo "Hello World"$'\n'

#command line args
echo "First argument: $1"
echo "Second argument: $2"
echo $'\n'

#creating file using a variable name
echo -n "Enter filename: "
read filename
touch "$filename.txt"
echo "File created."$'\n'

#concatenating strings
echo "Concatenating Strings"
s1="Good "
s2="Morning "
echo -n "Enter your name: "
read name
echo "$s1$s2$name"