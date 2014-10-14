#!/bin/bash
rm ./images/*
rm *.gif
g++ -Ofast tower_Of_Brahma.cpp
./a.out
chmod 755 script.sh
./script.sh
rm *.aux
rm *.log
rm a.out
rm script.sh