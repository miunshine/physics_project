#!/bin/bash

g++ -ggdb `pkg-config --cflags opencv` -o `basename preconditionCG .cpp` preconditionCG.cc BandMatrix.cc vector.cpp `pkg-config --libs opencv`

