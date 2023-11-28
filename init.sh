#!/bin/bash
sudo echo 1000 > /proc/sys/vm/nr_hugepages
sudo cpupower frequency-set --governor performance