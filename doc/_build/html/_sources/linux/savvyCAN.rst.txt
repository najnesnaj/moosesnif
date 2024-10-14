SavvyCAN
========

https://github.com/collin80/SavvyCAN

I used a prebuild version, since compiling QT stuff is not my cup.

SavvyCAN-x86_64.AppImage


using socketcan
---------------

recall : 

.. code-block:: none

   modprobe can
   modprobe can-raw
   modprobe slcan
   slcand -s6 -S2000000 /dev/ttyACM1 can0
   ifconfig can0 up

howto configure savvycan for use with socketcan
-----------------------------------------------

.. image:: ../images/savvycan-config.png



playback over socketcan
-----------------------

.. image:: ../images/savvycan-playback.png




