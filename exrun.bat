@echo off
set PROG=%1
if not exist %PROG% set PROG=Debug\%1
shift

set PATH=Debug;%PATH%
%PROG% %*
