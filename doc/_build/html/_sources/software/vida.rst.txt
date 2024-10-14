Volvo VIDA
==========

Volvo VIDA (Vehicle Information and Diagnostics for Aftersales) is a comprehensive diagnostic software system developed by Volvo Cars to aid in the servicing and maintenance of their vehicles. It provides technicians with the necessary tools to diagnose, troubleshoot, and repair Volvo cars effectively.

Key Features
------------

1. **Vehicle Information**: 
   VIDA provides detailed information about Volvo vehicles, including technical specifications, wiring diagrams, component locations, and service procedures. This allows technicians to quickly access relevant information needed for diagnosis and repair.

2. **Diagnostic Functions**: 
   VIDA allows technicians to perform comprehensive diagnostics on various vehicle systems, including the engine, transmission, ABS, airbag, climate control, and more. It can read and clear diagnostic trouble codes (DTCs), view live data streams, perform component tests, and program electronic control modules (ECUs).

3. **Software Updates**: 
   VIDA enables technicians to update vehicle software to the latest available versions. This is crucial for ensuring optimal performance, reliability, and compatibility with new technologies.

4. **Guided Diagnostics**: 
   VIDA offers guided diagnostic procedures to assist technicians in identifying and resolving vehicle issues efficiently. These step-by-step instructions help streamline the diagnostic process and minimize errors.

5. **Remote Diagnostics**: 
   VIDA supports remote diagnostics capabilities, allowing technicians to access vehicle data and perform diagnostics remotely. This feature can be particularly useful for troubleshooting complex issues or providing support to technicians in remote locations.

6. **Integrated Parts Catalog**: 
   VIDA includes an integrated parts catalog that provides access to genuine Volvo parts information, including part numbers, descriptions, and illustrations. This simplifies the process of ordering and replacing components during repairs.

7. **Multi-Language Support**: 
   VIDA is available in multiple languages, making it accessible to technicians worldwide.

Overall, Volvo VIDA is a powerful diagnostic tool that helps Volvo technicians efficiently diagnose and repair vehicles, ensuring optimal performance, safety, and reliability for Volvo owners.

databases
---------

The best way to get some relevant data out of the VIDA, is to start a diagnostic sessions. It then creates logfiles under : VIDA/system/log/diagnostics.


.. code-block:: none

     Trying to open a connection database server (Master DB)
      connection is open
     Found database AccessServer
     Found database BaseData
     Found database CarCom
     Found database DiagSwdlRepository
     Found database DiagSwdlSession
     Found database DiceTiming
     Found database imagerepository
     Found database EPC




SQL database VIDA
-----------------

the VIDA installation has a SQL server database (microsoft) onboard.
There is a way to peek into the tables :
- in the VidaConfigApplication.exe 
- DBUser :  sa  (in my case)
- DBpwd :  GunnarS3g3


SQL server import/export wizard 
- use sql Server Authentication
- select the database (eg DiagSwdlSession)


howto peek into the VIDA database:
----------------------------------

Volvo VIDA software is designed for multiple Volvo models and multiple model years. To select your model and the appropriate configuration, it reads out the VIN (vehicle identification number). This is made possible by the software's large database of XML files. There is a method to read out these XML files and select the ones that are important for your model.



`https://github.com/spnda/volvo_vida_db <https://github.com/spnda/volvo_vida_db>`_
`https://github.com/Tigo2000/Volvo-VIDA/ <https://github.com/Tigo2000/Volvo-VIDA/>`_

stored procedures:
------------------

in the microsoft sql database, you can find stored procedures, that can give some indication on where VIDA gets its information


.. code-block:: none

    /****** Object:  Stored Procedure dbo.vadis_GetDiagInit    Script Date: 2006-10-18 14:59:31 ******/
    
    ALTER  PROCEDURE [dbo].[vadis_GetDiagInit]
    @configId int
    AS
    SELECT     T132_InitValueType.name,
        T131_InitValue.initValue
    FROM
      T130_Init
    INNER JOIN
             T131_InitValue ON T130_Init.id = T131_InitValue.fkT130_Init
    INNER JOIN
             T132_InitValueType ON T131_InitValue.fkT132_InitValueType = T132_InitValueType.id
    INNER JOIN
      T121_Config ON T130_Init.id = T121_Config.fkT130_Init_Diag
    WHERE
     (T121_Config.id = @configId)
    

example:
--------

this could be a record of interest : 
CEM,Central electronic module (CEM),Read high beam relay,NA,372302,80,0x504B03041400000008000B1D47


- the number 372302 refers to the CEM
- 0x504.. is a coded file
