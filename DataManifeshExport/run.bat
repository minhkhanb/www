@echo off
call config.bat


cd %E_DIR_RELEASE%
%E_JAVA_HOME%\java -classpath .;%E_JXL% DataManifeshExport %E_DIR_DATA%\Objects.xls
cd %E_PRJ_HOME%

pause