#!/usr/bin/env bash
set -eu

if (( $# < 3 )); then
   echo "usage: $0 kp ki kd"
   exit 1
fi

simulator=$PWD/../../simulator/term2_sim_linux/term2_sim_linux/term2_sim.x86_64

cd build
cmake ..
make -j 4
if ! pgrep $(basename $simulator) &> /dev/null; then
   $simulator &
fi
./pid $1 $2 $3
