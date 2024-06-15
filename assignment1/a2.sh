#!/bin/bash

read -p "Enter basic salary: " basic
read -p "Enter increment rate: " rate

inc=$(((basic*rate)/100))
net=$((basic+inc))
echo "Basic salary: $basic"
echo "increment: $inc"
echo "Net salary: $net"
