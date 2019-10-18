#!/usr/bin/env bash
set -eu

if (( $# > 0 && $# < 3 )); then
   echo "usage: $0 kp ki kd"
   exit 1
fi

if [[ $(uname -s) == Darwin ]]; then
   simulator=$PWD/../../simulator/term2_sim_mac/term2_sim_mac/term2_sim.app/Contents/MacOS/term2_sim_mac
else
   simulator=$PWD/../../simulator/term2_sim_linux/term2_sim_linux/term2_sim.x86_64
fi

cd build
cmake ..
make -j 4
if ! pgrep $(basename $simulator) &> /dev/null; then
   $simulator &
fi

if (( $# == 3 )); then
   ./pid $1 $2 $3
else
   ./pid
fi
