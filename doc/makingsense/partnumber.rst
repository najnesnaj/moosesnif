Reading a partnumber
====================



Reading part number from ECU 0x50
---------------------------------

.. code-block:: none

   ---> ID=000ffffe data=50 88 00 00 00 00 00 00
   <--- ID=00000003 data=50 8e 00 00 31 25 49 03
   Part Number: 000031254903

.. code-block:: none

    CAN_HS ---> ID=000ffffe data=cb 50 b9 f0 00 00 00 00
    CAN_HS <--- ID=01000003 data=8f 50 f9 f0 00 08 68 85
    CAN_HS <--- ID=01000003 data=09 13 20 20 41 30 65 70
    CAN_HS <--- ID=01000003 data=4c 24 20 20 41 00 00 00

First byte in reply is a technical field, like rolling counter, start (0x80)/end(0x40 flag of the multipart message, etc. The P/N is:

08 68 85 13 == 8688513

quick analysis
--------------

data = cb 50 b9 f0

- length = 3 (cb-c8)
- module = 50 (CEM)
- type of operation = b9 (read datablock by offset)
-  






ECU Identification
------------------

.. code-block:: none

    B9F0
      can0  00400003   [8]  8F 50 F9 F0 00 08 69 07
      can0  00400003   [8]  09 19 20 20 20 30 72 89
      can0  00400003   [8]  4C 79 20 41 41 00 00 00
    
    B9F1 - B9F4
      can0  00400003   [8]  CC 50 7F B9 12 00 00 00
    
    
Downloadable Software Part Number
---------------------------------

.. code-block:: none

    B9F5
      can0  00400003   [8]  8F 50 F9 F5 00 30 78 63
      can0  00400003   [8]  09 53 20 41 41 00 00 C0
      can0  00400003   [8]  0A 00 00 30 78 62 13 20
      can0  00400003   [8]  0B 41 41 00 00 40 00 00
      can0  00400003   [8]  0C 30 72 89 81 20 41 41
      can0  00400003   [8]  4C 00 00 70 00 00 00 00
    
hex2human
---------

CAN Config Part Number: 0030786353204141 (30786353 AA)
Flash Sector Start Address: 0000C000

Application Software Part Number: 0030786213204141 (30786213 AA)
Flash Sector Start Address: 00004000

Local Config Part Number: 0030728981204141 (30728981 AA)
Flash Sector Start Address: 00007000
