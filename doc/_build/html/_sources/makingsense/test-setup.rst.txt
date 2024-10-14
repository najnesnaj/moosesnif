test-setup
----------
I used a setup on a stm32 to simulate the accelerator pedal.

**stm32f103c6-canbus**


This microcontroller sends values to the CANBUS, which can be picked up by the dice software.

In the dice diagnostic software this parameter can be display graphically. (graph)

.. code-block:: none

                frame.id = 0x00222222; //pretend this is the CEM module
                frame.extended_id = true; //volvo uses 29bit
                frame.rtr = false;
                frame.len = 8;
                // Populate the payload bytes with accelerator pedal 0x3F92
                frame.data[0] = 0xCE; //length = 5
                frame.data[1] = 0x50; //CEM module
                frame.data[2] = 0xE6; //reply to A6
                frame.data[3] = 0x3F; //accel pedal
                frame.data[4] = 0x92; //16bit value
                frame.data[5] = 0x24; //value
                frame.data[6] = 0x01; //number of messages
                frame.data[7] = 0x00;
                CAN::transmit(frame);

