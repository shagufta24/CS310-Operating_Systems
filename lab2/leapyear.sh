#!/bin/bash

echo "Enter a year: "
read year

if []
then
    if [$(($year % 4)) -eq 0]
    then
        echo Leap year
    else
        echo Not a Leap year
    fi
else
echo Enter a valid year
fi