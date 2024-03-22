target extended-remote /dev/ttyACM0
monitor version
monitor swdp_scan
attach 1
set mem inaccessible-by-default off
