# This includes sqlplus.spec _and_ sqlplus.spec.in 
#   (this way rpm -ta xxxx work)
# Added mysql-config(.in) so that mysql.m4 will work (these should be
#   moved to the mysql distribution, of course.
EXTRA_DIST = @PACKAGE@.spec @PACKAGE@.spec.in @PACKAGE@-config.in \
  mysql-config mysql-config.in \
  autogen.sh COPYRIGHT ChangeLog README AUTHORS NEWS

SUBDIRS = sqlplusint examples doc macros # devel

pkginclude_HEADERS = mysql++-config.hh

# For now, mysql-config is "hardcoded" though the ".in" version exists
bin_SCRIPTS = mysql++-config mysql-config

@PACKAGE@-config: @PACKAGE@-config.in

if MAKE_RPMS
rpm: @RPM_TARGET@

.PHONY: rpm

@RPM_TARGET@: $(DISTFILES)
	${MAKE} dist
	@RPM_PROG@ @RPM_ARGS@ @RPM_TARBALL@
	@echo Congratulations, @RPM_TARGET@ "(and friends)" should now exist.
else
endif
