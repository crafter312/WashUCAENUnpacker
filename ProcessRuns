#!/bin/bash

for runnum in "$@" 
do
	./sort $runnum
	root -l <<END
SaveRun($runnum)
.q
END
	echo "Run $runnum saved";
done
