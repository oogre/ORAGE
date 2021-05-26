#!/bin/bash

killall OrageApp;

cd ~/Forks/ORAGE/proj/cmake/build;
cmake ../;
make;
if [ $? -eq 0 ]; then
	./Debug/OrageApp/OrageApp;
fi

