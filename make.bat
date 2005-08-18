@echo off

rem Command line parsing
:reparse
if not exist lib\Makefile goto error
if "%1" == "clean" goto do_clean
if "%1" == "install" goto do_install


rem Build the library file
cmd /c cd lib ^& make.bat
if errorlevel 1 goto end
echo.
echo MySQL++ library built successfully.
copy lib\*.dll examples > NUL

cmd /c cd examples ^& make.bat %*
if errorlevel 1 goto end
echo.
echo MySQL++ examples built successfully.
goto end


rem Remove generated files
:do_clean
if not exist lib\Makefile goto error
if not exist examples\Makefile goto error
cd lib
call make.bat clean
del Makefile make.bat
cd ..
cd examples
call make.bat clean
del Makefile make.bat
cd ..
shift
if "%1" NEQ "" goto reparse
goto end


rem Install library file
:do_install
cd lib
call make.bat install
cd ..
shift
if "%1" NEQ "" goto reparse
goto end


rem Display error message
:error
echo.
echo You must run makemake first!
goto end


:end
