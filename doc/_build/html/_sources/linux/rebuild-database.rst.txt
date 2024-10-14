Using mssql database on linux
=============================

Turns out it is possible to use ms-sqlserver under linux

https://learn.microsoft.com/en-us/sql/linux/quickstart-install-connect-ubuntu?view=sql-server-ver16&tabs=ubuntu2004

controlling the beast ....

.. code-block:: none

    systemctl status mssql-server
    systemctl stop mssql-server
    systemctl disable mssql-server
    systemctl enable mssql-server


A nice GUI would be cool!!

https://dbeaver.io
sudo add-apt-repository ppa:serge-rider/dbeaver-ce
sudo apt-get update
sudo apt-get install dbeaver-ce



And now .. a way to get Volvo vida files accessible under linux.
This allows for exporting data and having access to stored procedures. (the tools on windows seemed rather limiting...)

.. code-block:: none

    /opt/mssql-tools/bin/sqlcmd -S . -U sa -P GunnarS3g3 -Q "CREATE DATABASE BaseData_Data ON (FILENAME = '/var/opt/mssql/data/BaseData_Data.MDF'),(FILENAME = '/var/opt/mssql/data/BaseData_Data_log.ldf' ) FOR ATTACH_REBUILD_LOG"

    /opt/mssql-tools/bin/sqlcmd -S . -U sa -P GunnarS3g3 -Q "CREATE DATABASE CARCOM ON (FILENAME = '/twee/volvo/CarComRT_Data.MDF'),(FILENAME = '/twee/volvo/CarcomRT_Log.LDF' ) FOR ATTACH_REBUILD_LOG"

    /opt/mssql-tools/bin/sqlcmd -S . -U sa -P GunnarS3g3 -Q "CREATE DATABASE DIAGSWDLREPOSITORY ON (FILENAME = '/twee/volvo/DiagSwdlRepository_Data.MDF'),(FILENAME = '/twee/volvo/DiagSwdlRepository_log.LDF ' ) FOR ATTACH"

    /opt/mssql-tools/bin/sqlcmd -S . -U sa -P GunnarS3g3 -Q "CREATE DATABASE DIAGSWDLSESSION ON (FILENAME = '/twee/volvo/DiagSwdlSession_Data.MDF'),(FILENAME = '/twee/volvo/DiagSwdlSession_log.LDF' ) FOR ATTACH"


In Dbeaver the stored procedure can be executed, by right clicking.
It then opens a new windows, where you can add the parameter.

To execute a stored procedure in SQL Server that requires a parameter, you can use the EXEC statement followed by the name of the stored procedure and provide the parameter value within parentheses. Here's how you can pass a parameter to the stored procedure [dbo].[vadis_GetEcuVariantData]:


.. code-block:: none

   USE [CARCOM]
   DECLARE @return_value int
   DECLARE @parameter_name int = 821
   EXEC	@return_value = [dbo].[vadis_GetEcuVariantData]@parameter_name

   SELECT	'Return Value' = @return_value

it return DiagNumber : 30728270 AA 

+-------------+----------+-----+----+-----+-----+-------+-----+------------+
| DiagNumber  | EcuTypeI | EcuI| Add| Ecu | Nam | CfgId | Gwy | ProtocolId |
+=============+==========+=====+====+=====+=====+=======+=====+============+
| 30728270 AA | 372,302  | CEM | 52 | 821 | CEM | 611   |     |    7       |
+-------------+----------+-----+----+-----+-----+-------+-----+------------+

