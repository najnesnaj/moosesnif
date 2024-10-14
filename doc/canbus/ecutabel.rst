Modules for profile
===================

In the file : /system/log/diagnostics/VIDA, is a lot of usefull info.

For instance : vadis_GetallEcuDataForProfile 0b00c8af83f1a8fb.


executing stored procedure
--------------------------


SELECT  DISTINCT
			 t121.commaddress as ecuaddress,
			 t123.identifier as busid,
			 t102.identifier as ecutype,
			 t102.description ecuDescription,
			 t100.identifier diagnosticnumber,
			 t121.id as configId,
			 t121.canidrx as canid,
			 t121.fkT121_Config_Gateway as gatewayConfigId,
			 t122.id as protocolid,
			 t161.folderLevel as folderLevel,
			 t121.priority as [priority]
			FROM      T120_Config_EcuVariant t120
			INNER JOIN
			   T121_Config T121 ON T120.fkT121_Config = T121.id
			INNER JOIN
			   T122_Protocol T122 ON T121.fkT122_protocol = T122.id
			INNER JOIN
			   T100_EcuVariant T100 ON T120.fkT100_EcuVariant = T100.id
			INNER JOIN
			   T160_DefaultEcuVariant T160 ON T100.id = T160.fkT100_EcuVariant
			INNER JOIN
			   T161_Profile T161 on T160.fkT161_Profile = T161.Id
			INNER JOIN
			   T123_Bus t123 on T121.fkt123_bus = t123.id
			INNER JOIN
			   T101_Ecu t101 on T100.fkt101_ecu = t101.id
			INNER JOIN
			   T102_EcuType t102 on t101.fkt102_ecutype = t102.id
			INNER JOIN
			  dbo.GetCompatibleProfiles('0b00c8af83f1a8fb') p on t161.id = p.id
			WHERE
			 t102.identifier <> 0
			ORDER BY t161.folderLevel DESC, t121.priority ASC

table with overview
-------------------
"ecuaddress","busid","ecutype","ecuDescription",
"diagnosticnumber","configId","gatewayConfigId"

.. csv-table:: EcuAddress for Model
   :header: "Address", "busid", "ecutype", "descript"
   :widths: 1, 1, 2, 1

   "0B", 1, 254501, "ADM"
   "11", 1, 284101, "ECM"
   "7B", 0, 372306, "AUU"
   "7C", 0, 394202, "BPM"
   "2D", 0, 366901, "KVM"
   "63", 0, 366902, "PAM"
   "52", 0, 372301, "AEM"
   "40", 0, 372304, "CEM"
   "51", 0, 381101, "DIM"
   "6D", 0, 393202, "AUD"
   "75", 0, 393204, "IAM"
   "71", 0, 393205, "RDAR"
   "54", 0, 393901, "ICM"
   "66", 0, 393906, "MMM"
   "64", 0, 394201, "PHM"
   "68", 0, 395301, "SUB"
   "43", 0, 831701, "DDM"
   "45", 0, 831703, "PDM"
   "2E", 0, 852601, "PSM"
   "29", 0, 871101, "CCM"
   "18", 0, 875401, "CPM"
   "58", 0, 884701, "SRS"
   "23", 0, 892901, "TRM"
   "70", 1, 352902, "HCM"
   "50", 1, 372302, "CEM"
   "01", 1, 593102, "BCM"
   "49", 1, 641901, "SWM"
   "30", 1, 645901, "EPS"


