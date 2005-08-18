@echo off

rem Figure out which compiler the user wants to create Makefiles for
if "%1" == "vc" goto build_makefiles
if "%1" == "bc" goto build_makefiles
if "%1" == "mingw" goto build_makefiles
goto usage


rem Set up common headers for new Makefiles
:build_makefiles
set head1=## DO NOT EDIT.  Created from Makefile.%1 and Makefile.base.
set head2=## Edit those files instead, then type 'makemake %1' to rebuild
set head3=## this Makefile.

rem Decide how to build subordinate make.bat files
if "%1" == "vc" goto vc_make
goto generic_make
:vc_make
echo @nmake /nologo %%* > lib\make.bat
echo @nmake /nologo %%* > examples\make.bat
goto make_bat_created
:generic_make
echo @make.exe %%* > lib\make.bat
echo @make.exe %%* > examples\make.bat

:make_bat_created
rem Create library Makefile
echo %head1% > lib\Makefile
echo %head2% >> lib\Makefile
echo %head3% >> lib\Makefile
echo. >> lib\Makefile
echo. >> lib\Makefile
type lib\Makefile.%1 >> lib\Makefile
type lib\Makefile.base >> lib\Makefile

rem Create example programs Makefile
echo %head1% > examples\Makefile
echo %head2% >> examples\Makefile
echo %head3% >> examples\Makefile
echo. >> examples\Makefile
echo. >> examples\Makefile
type examples\Makefile.%1 >> examples\Makefile
type examples\Makefile.base >> examples\Makefile
shift
call make.bat
goto end


rem Display usage message
:usage
echo usage: makemake {vc, bc, mingw} [args]
echo.
echo     You must give one of the compiler parameters:
echo.
echo         vc: Visual C++ command-line compiler (cl) and nmake
echo         bc: Borland C++ command-line compiler (bcc32) and Borland make
echo         mingw: MinGW GCC (g++) and GNU make
echo.
goto end


:end
