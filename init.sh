#!/bin/bash
sudo su
echo 1000 > /proc/sys/vm/nr_hugepages
cpupower frequency-set --governor performance
exit