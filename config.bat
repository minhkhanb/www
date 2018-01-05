SET PRJ_DIR=%CD%
SET J2SE_DIR=%JAVA_SDK%

SET TOOLS_PATH=%PRJ_DIR%\tools
SET PROGUARD=%TOOLS_PATH%\Proguard\proguard.jar
SET TEXT_PACK=%TOOLS_PATH%\TextPacker.jar
SET ANIM_EXPORT=%TOOLS_PATH%\PineSpriteEditor\PineSpriteEditor.exe


SET SRC=%PRJ_DIR%\src
SET RES=%PRJ_DIR%\res
SET SRC_DIR_CPP=%PRJ_DIR%\src\game\GamePlay
SET SRC_DIR_H=%PRJ_DIR%\src\game\Define
SET TMPRES=%PRJ_DIR%\_tmpres
SET BIN=%PRJ_DIR%\bin
SET RES_DES=%BIN%\res
SET DESIGN_FILE="%PRJ_DIR%\res\DataManifesh.xls"
SET DATA_EXPORT_TOOL_DIR="%PRJ_DIR%\DataManifeshExport"
SET DESIGN_RELEASE_DIR=%PRJ_DIR%\src\game\GamePlay\GameDesign
SET OBJECT_RELEASE_DIR=%PRJ_DIR%\src\game\GamePlay\GameObject

::Define for Android
SET ANDROID_PRJ_DIR=%PRJ_DIR%\src\_android
SET ANDROID_RES=%ANDROID_PRJ_DIR%\assets

REM ///export design
SET DESIGN_EXPORT_DIR=%PRJ_DIR%\Tools\GDS_Exporter

SET DESIGN_FILE_EXPORT="%PRJ_DIR%\doc\ExportDesign\GDS_Exporter.conf"
SET DESIGN_FILE_CPP="%PRJ_DIR%\doc\ExportDesign\GDS_Exporter.cpp"
SET DESIGN_FILE_H="%PRJ_DIR%\doc\ExportDesign\GDS_Exporter.h"
SET SRC_FILE_H="%PRJ_DIR%\doc\ExportDesign\GDS_Exporter.java"

