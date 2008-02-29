@echo off
rem This copies the built MySQL++ DLL into the Windows-specific
rem examples' build directories so they can run without hackery.
copy debug\*.dll examples\vstudio\debug > NUL
copy release\*.dll examples\vstudio\release > NUL
