#!/usr/bin/env bash
set -eu

simulator=$PWD/../../simulator/term2_sim_linux/term2_sim_linux/term2_sim.x86_64

cd build
cmake ..
make -j 4
if ! pgrep $(basename $simulator) &> /dev/null; then
   $simulator &
fi
./pid
