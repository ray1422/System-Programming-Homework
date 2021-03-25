#!/bin/bash
for i in {2..10}; do
	set BUF_SIZE=$(echo "${i}" | bc)
	echo "buf size: $BUF_SIZE" # > report_${BUF_SIZE}.txt
	echo strace # >> report_${BUF_SIZE}.txt
	strace -c ./fileperf src dst $BUF_SIZE
	echo ltrace # >> report_${BUF_SIZE}.txt
	ltrace -c ./fileperf src dst $BUF_SIZE
done

