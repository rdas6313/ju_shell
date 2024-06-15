#!/bin/sh

read -p "Enter length: " a
read -p "Enter breadth: " b
peri=$((2*(a+b)))
area=$((a*b))
echo "Perimeter: " $peri
echo "Area: " $area

