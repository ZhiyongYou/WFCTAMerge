#!/bin/bash
for iday in `seq 101 131`
do
	iday=`printf %04d $iday`
	./creatjob.sh $iday
done
