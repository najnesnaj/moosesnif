using    candump
================

on linux, first load the modules, then start the deamon with 500K baud (-s6)

.. code-block:: none

   modprobe can
   modprobe can-raw
   modprobe slcan
   slcand -s6 -S2000000 /dev/ttyACM1 can0
   ifconfig can0 up

using DICE cable and modding OpenMoose
--------------------------------------

after modifying OpenMoose, I get to see my custom-message (22,23,24,25,26,27,28)
which proves that the DICE cable can be used!

.. code-block:: none

   root@naj-Latitude-5520:~# candump can0 -ta
    (1710516027.391935)  can0  000FFFFE   [8]  D8 00 00 00 00 00 00 00
    (1710516028.391723)  can0  000FFFFE   [8]  D8 00 00 00 00 00 00 00
    (1710516028.594302)  can0  000FFFFE   [8]  16 17 18 19 1A 1B 1C 1D
    (1710516029.391926)  can0  000FFFFE   [8]  D8 00 00 00 00 00 00 00


