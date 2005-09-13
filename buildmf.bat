@echo off
set head1=## DO NOT EDIT.  Created from Makefile.%1 and Makefile.base.
set head2=## Edit those files instead, then type 'makemake %1' at the
set head3=## project root to rebuild this Makefile.

echo %head1% > %2\Makefile
echo %head2% >> %2\Makefile
echo %head3% >> %2\Makefile
echo. >> %2\Makefile
echo. >> %2\Makefile
type %2\Makefile.%1 >> %2\Makefile
echo. >> %2\Makefile
echo. >> %2\Makefile
type %2\Makefile.base >> %2\Makefile

