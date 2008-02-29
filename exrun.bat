@echo off
set PROG=%1
if not exist %PROG% set PROG=%1.exe
if not exist %PROG% set PROG=Debug\%1
if not exist %PROG% set PROG=Debug\%1.exe
shift

set PATH=Debug;%PATH%
%PROG% %1 %2 %3 %4 %5 %6 %7 %8 %9
