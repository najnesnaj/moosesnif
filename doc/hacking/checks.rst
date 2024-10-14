Checks between the ECM, CEM and BCM
=====================================

The check between the ECM, CEM, and BCM occurs when the ignition key is turned to position II (takes approximately 100 ms). After the CEM has approved the key, the ECM is informed that this is the case.

The following checks occur before the engine is started:

Check 1
-------
The ECM transmits a request to the BCM to transmit its serial number. The ECM checks that the serial number corresponds to the serial number it has stored. If the serial number is correct, the start conditions have been met.

Check 2
-------
At the same time as the ECM checks the BCM, the ECM transmits a random number and a code to the CEM. The CEM checks that the code corresponds to the code it has stored. If the code is correct, the CEM transmits another code back to the ECM. The ECM checks the code and compares it to the code it has stored. If it is correct, another start condition has been met.

When Check 1 and Check 2 are complete and approved, the CEM permits the ECM to activate the relays for the starter motor and the engine can be started.


from the manual

Checks between the ECM, CEM and BCM
The check between the ECM, CEM and BCM occurs when the ignition key is turned to position II (takes
approximately 100 ms).
After the CEM has approved the key, the ECM is informed that this is the case.
The following checks occur before the engine is started:
Check 1
The ECM transmits a request to the BCM to transmit its serial number. The ECM checks that the serial
number corresponds to the serial number it has stored. If the serial number is correct the start
conditions have been met.
Check 2
At the same time as the ECM checks the BCM, the ECM transmits a random number and a code to the
CEM. The CEM checks that the code corresponds to the code it has stored. If the code is correct the
CEM transmits another code back to the ECM. The ECM checks the code and compares it to the code
it has stored. If it is correct another start condition has been met.
When Check 1 and Check 2 are complete and approved the CEM permits the ECM to activate the
relays for the starter motor and the engine can be started.
