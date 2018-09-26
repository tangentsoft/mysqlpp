SHELL := $(COMSPEC)
MYSQL_DIR := "c:/Program Files/MySQL/MySQL Connector C 6.1"
CXXFLAGS := -I$(MYSQL_DIR)/include -Ic:/MySQL++/include
LDFLAGS := -L$(MYSQL_DIR)/lib/opt -Lc:/MySQL++/lib/MinGW
LDLIBS := -lmysqlclient -lmysqlpp
EXECUTABLE := hello

all: $(EXECUTABLE)

clean: 
	del $(EXECUTABLE)
