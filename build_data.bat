@echo off
call config.bat

echo *** Creating directories ***
mkdir %TMPRES%
mkdir %BIN%
mkdir %RES_DES%
cd %DATA_EXPORT_TOOL_DIR%

echo *** Build Data ***
REM cd %DESIGN_EXPORT_DIR%
copy  %DESIGN_FILE% %DATA_EXPORT_TOOL_DIR%\data\Objects.xls
REM cd %DESIGN_EXPORT_DIR%
REM echo %cd%
call make.bat
call run.bat
cd %DATA_EXPORT_TOOL_DIR%\release

copy SpriteList.h %OBJECT_RELEASE_DIR%
copy lib_SpriteList.cpp %OBJECT_RELEASE_DIR%
copy *.h %DESIGN_RELEASE_DIR%
copy *.manifest %RES%

echo *** Create New FileVersion ***
mkdir %BIN%\FileVersion
copy Ver_* %BIN%\FileVersion
echo *** FileVersion create completed ***

cd %TOOLS_PATH%

call copy_files.bat %RES% %TMPRES%
cd %TMPRES%
rem pack text

rem pack animation
for %%i in (*.pine) do (
	%ANIM_EXPORT% 	-i %%i		-o %%~ni.bpine  QUALITY_TRUE_COLOR		PIXEL_16BITS	EXPORT_AS_3D_TEXTURE	EXPORT_FMODULE_OPACITY 	USE_MULTI_MODULE_TYPE 	EXPORT_PALETTE_COLOR
)


copy *.h %SRC_DIR_H%

copy %PRJ_DIR%\res\gfx\bg\bg01_layer3.png  %RES_DES%\bg01_layer3.pbg
copy %PRJ_DIR%\res\gfx\bg\bg01_layer3.png  %BIN%\Res_iOS\bg01_layer3.pbg

rem del /Q *.*
REM cd %PRJ_DIR%
REM mkdir ##advert_release

REM cd %PRJ_DIR%\pine_advert_plugin\PinePromotionExporter
REM call make.bat
REM call run.bat
REM copy release\*.bpine %PRJ_DIR%\##advert_release
REM copy release\pinegame_promotion_info.pine %PRJ_DIR%\##advert_release

REM copy release\*.bpine %BIN%
REM copy release\pinegame_promotion_info.pine %BIN%

cd %PRJ_DIR%
rem rd %TMPRES%

call deploy_data_ios.bat
echo *** Building Data Complete ***

pause