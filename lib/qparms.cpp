## This file contains the 'make' rules specific to Visual C++.  The
## ../makemake.bat script prepends this to Makefile.base to create a
## complete Makefile.

# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with MySQL++; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
# USA

MYSQL_DIR=c:\mysql

CYGWIN_MAKE=$(wildcard /bin/cygwin1.dll)

BIN_DIR=debug
EX_BIN_DIR=$(if $(CYGWIN_MAKE),../examples/$(BIN_DIR)/,..\examples\$(BIN_DIR)\)

CXX=cl
CXXFLAGS=/I$(MYSQL_DIR)\include /DMYSQLPP_MAKING_DLL /D_WINDLL /D_UNICODE \
		/G6 /EHsc /nologo /c

LIB_BASE=mysqlpp
LIB_FILE=$(LIB_BASE).dll
IMP_FILE=$(LIB_BASE).lib
VC_LIB_PATH=$(BIN_DIR)\$(LIB_FILE)
PLAT_LIB_PATH=$(if $(CYGWIN_MAKE),\
		$(shell cygpath -u $(VC_LIB_PATH)),\
		$(VC_LIB_PATH))

LD=link
LDFLAGS=/OUT:$(VC_LIB_PATH) /DLL /NOLOGO \
		/LIBPATH:$(MYSQL_DIR)\lib\opt libmysql.lib

ifeq "$(BIN_DIR)" "debug"
	CXXFLAGS += /MDd /Od /D_DEBUG /ZI /Zi
	LDFLAGS += /DEBUG
else
	CXXFLAGS += /MD /O2
endif

vpath %.obj $(BIN_DIR)
vpath %.dll $(BIN_DIR)
vpath %.lib $(BIN_DIR)

EXE=.exe
OBJ=obj

RM=$(if $(CYGWIN_MAKE),rm -rf,del /q)
CP=$(if $(CYGWIN_MAKE),cp,copy)

LOCAL_CLEAN=debug release


.SUFFIXES: .cpp .obj
.cpp.obj:
	$(CXX) $(CXXFLAGS) /Fo$(BIN_DIR)\$@ $<


vc: all

example_setup:
	$(CP) $(PLAT_LIB_PATH) $(EX_BIN_DIR)

$(IMP_FILE):
	$(if $(wildcard $(BIN_DIR)/$(IMP_FILE)),,$(shell $(RM) $(BIN_DIR)/$(LIB_FILE)))
	$(MAKE) $(LIB_FILE)
