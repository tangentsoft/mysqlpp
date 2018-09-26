@echo off
if "%1" == "" goto error

set BASE_DIR=c:\mysql++
set INST_INC_DIR=%BASE_DIR%\include
set INST_LIB_DIR=%BASE_DIR%\%1

if not exist %BASE_DIR% mkdir %BASE_DIR%
if not exist %INST_INC_DIR% mkdir %INST_INC_DIR%
if not exist %INST_LIB_DIR% mkdir %INST_LIB_DIR%
if not exist %INST_LIB_DIR%\debug mkdir %INST_LIB_DIR%\debug
if not exist %INST_LIB_DIR%\release mkdir %INST_LIB_DIR%\release

copy lib\*.h "%INST_INC_DIR%" > NUL

if exist *.a goto install_mingw
copy debug\*.dll "%INST_LIB_DIR%\debug" > NUL
copy debug\*.lib "%INST_LIB_DIR%\debug" > NUL
copy release\*.dll "%INST_LIB_DIR%\release" > NUL
copy release\*.lib "%INST_LIB_DIR%\release" > NUL
goto install_done
:install_mingw
copy *.a "%INST_LIB_DIR%\debug" > NUL
echo WARNING: I assume you built a debug version of the library, as that
echo is what you get with MinGW unless you make a special effort.  You
echo must do a manual install if you make a release version.

:install_done
echo MySQL++ (%1 version) installed successfully!
goto end

:error
echo usage: install [subdir]
echo.
echo 	Installs MySQL++ into the given subdirectory of %BASE_DIR%

:end

