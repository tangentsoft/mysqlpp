# This includes sqlplus.spec _and_ sqlplus.spec.in 
#   (this way rpm -ta xxxx work)
EXTRA_DIST = @PACKAGE@.spec @PACKAGE@.spec.in @PACKAGE@-config.in

SUBDIRS = sqlplusint examples doc # devel

bin_SCRIPTS = sqlplus-config

CONFIG_FILE = @PACKAGE@-config-@SQLPLUS_MAJOR_VERSION@.@SQLPLUS_MINOR_VERSION@

$(CONFIG_FILE): @PACKAGE@-config
	cp $^ $@

@PACKAGE@-config: @PACKAGE@-config.in
