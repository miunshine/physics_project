#!/bin/bash

g++ -ggdb -O2 `pkg-config --cflags opencv` -o `basename BiCGSTAB .cpp` BiCGSTAB.cc BandMatrix.cc vector.cpp `pkg-config --libs opencv`

