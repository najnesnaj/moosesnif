Carcommunication and scripts
============================

in the CARCOM database (T102), the modules are defined by a number:
example the CEM = 372302

**scripttype statusindentifier**

in the DIAGSWDL database (ScriptCarFunction), Functiongroup (372302 exactly the same number refers to numerous script numbers)

Example : 0900c8af81b6a712 which is SUN_SENSOR,_VOLTAGE,_STATUS

in the XML file : 
-----------------

.. code-block:: none

    property class="Parameter" id="nev9960842n1-nev9960863n1"
    identifier ecu="372304" ecuMode="EcuType" id="nev9960842n1-nev9960865n1" read="4" type="REID" value="5E01"

.. image:: ../images/ScriptCarFunction.png


ScriptType
----------

Checking for our purpose VehCommSpecification:

VehCommSpecification
EcuIdentification
CarStatus
CarConfig
ClearDtc
ReadVin
StatusIdentifier
ReadDtc
ReadDtcEcm
Dro
TransportMode
CustomerParameter
PreProg
EcuReplace
PreCompleteReadout
AlarmMode
KeyPos
DownloadMode
EcuIdentificationHighSpeed
EcuIdentificationLowSpeed
ClockValue
PostReset
PreSwdl

