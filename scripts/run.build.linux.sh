#!/bin/bash

while inotifywait -r ~/Forks/ORAGE/src -e modify,create,delete,moved_to;
do 
	~/Forks/ORAGE/scripts/build.linux.sh &
done
