include ../Configure

ifeq "$(findstring shared, $(TYPE))" "shared"
  CFLAGS += $(FPIC)
endif

ifeq "$(findstring static, $(TYPE))" "static"
  INTLIBS := libmysql++.a
endif
ifeq "$(findstring shared, $(TYPE))" "shared"
  INTLIBS := $(INTLIBS) libmysql++.so libmysql++.so.1
endif

objs: $(OBJSINT)

static-lib: libmysql++.a
shared-lib: libmysql++.so libmysql++.so.1

libmysql++.a: $(OBJSINT)
	$(ARC) libmysql++.a $(OBJSINT)
	ranlib libmysql++.a 

libmysql++.so libmysql++.so.1: $(OBJSINT)
	$(LINK) -shared -o libmysql++.so.1 -W1,-soname,libmysql++.so.1 $(OBJSINT)
	-rm libmysql++.so
	-ln -s libmysql++.so.1 libmysql++.so

clean:
	-rm -f $(OBJSINT) doc.tex

realclean: clean
	-rm -f $(INTLIBS) 

perlclean: realclean
	-rm -f custom-macros.hh custom.hh

custom.hh custom-macros.hh: custom.pl
	perl custom.pl

install: $(HEADSINT) $(INTLIBS)
	-mkdir $(INSTHEAD)
	-mkdir $(INSTHEAD)/mysql++-int
	$(CP_R) -f $(HEADSINT) $(INSTHEAD)/mysql++-int/
	-mkdir $(INSTLIB)
ifeq "$(findstring static, $(TYPE))" "static"
	$(CP_R) -f $(INTLIBS) $(INSTLIB)
else
ifeq "$(findstring shared, $(TYPE))" "shared"
	$(CP_R) -f $(INTLIBS) $(INSTLIB)
endif
endif

remove:
	-rm -f $(INTLIBS:%=$(INSTLIB)/%)
	-$(RMDIR) $(INSTLIB)
	-rm -f $(HEADSINT:%=$(INSTHEAD)/mysql++-int/%)
	-$(RMDIR) $(INSTHEAD)/mysql++-int
	-$(RMDIR) $(INSTHEAD)

classref: *1.hh
	-rm -f .perceps doc.sgml
	perceps -N *1.hh

coldata.o: coldata.cc const_string3.hh const_string2.hh \
 const_string1.hh coldata4.hh convert3.hh convert2.hh convert1.hh \
 coldata3.hh coldata2.hh coldata1.hh defs define_short type_info1.hh \
 null1.hh null3.hh null2.hh
connection.o: connection.cc connection3.hh connection2.hh \
 connection1.hh define_short query1.hh sql_query1.hh sql_query0.hh \
 sql_string1.hh defs result1.hh field_names1.hh coldata1.hh \
 type_info1.hh const_string1.hh null1.hh row1.hh resiter1.hh \
 vallist1.hh manip1.hh datetime1.hh stream2string1.hh tiny_int1.hh \
 set1.hh tracker.h field_types1.hh fields1.hh connection0.hh query2.hh \
 coldata2.hh result3.hh result2.hh fields2.hh
datetime.o: datetime.cc datetime3.hh datetime2.hh datetime1.hh defs \
 define_short coldata1.hh type_info1.hh const_string1.hh null1.hh \
 stream2string1.hh tiny_int1.hh coldata2.hh stream2string2.hh
field_names.o: field_names.cc field_names3.hh field_names2.hh \
 field_names1.hh defs define_short coldata1.hh type_info1.hh \
 const_string1.hh null1.hh result2.hh connection2.hh connection1.hh \
 query1.hh sql_query1.hh sql_query0.hh sql_string1.hh result1.hh \
 row1.hh resiter1.hh vallist1.hh manip1.hh datetime1.hh \
 stream2string1.hh tiny_int1.hh set1.hh tracker.h field_types1.hh \
 fields1.hh connection0.hh query2.hh coldata2.hh fields2.hh
field_types.o: field_types.cc field_types3.hh field_types2.hh \
 field_types1.hh defs define_short type_info1.hh result2.hh \
 connection2.hh connection1.hh query1.hh sql_query1.hh sql_query0.hh \
 sql_string1.hh result1.hh field_names1.hh coldata1.hh \
 const_string1.hh null1.hh row1.hh resiter1.hh vallist1.hh manip1.hh \
 datetime1.hh stream2string1.hh tiny_int1.hh set1.hh tracker.h \
 fields1.hh connection0.hh query2.hh coldata2.hh fields2.hh
manip.o: manip.cc manip3.hh manip2.hh manip1.hh define_short \
 sql_string1.hh defs sql_query1.hh sql_query0.hh datetime1.hh \
 coldata1.hh type_info1.hh const_string1.hh null1.hh stream2string1.hh \
 tiny_int1.hh set1.hh
query.o: query.cc query3.hh query2.hh query1.hh define_short \
 sql_query1.hh sql_query0.hh sql_string1.hh defs result1.hh \
 field_names1.hh coldata1.hh type_info1.hh const_string1.hh null1.hh \
 row1.hh resiter1.hh vallist1.hh manip1.hh datetime1.hh \
 stream2string1.hh tiny_int1.hh set1.hh tracker.h field_types1.hh \
 fields1.hh connection0.hh connection1.hh connection2.hh coldata2.hh
result.o: result.cc result3.hh result2.hh connection2.hh \
 connection1.hh define_short query1.hh sql_query1.hh sql_query0.hh \
 sql_string1.hh defs result1.hh field_names1.hh coldata1.hh \
 type_info1.hh const_string1.hh null1.hh row1.hh resiter1.hh \
 vallist1.hh manip1.hh datetime1.hh stream2string1.hh tiny_int1.hh \
 set1.hh tracker.h field_types1.hh fields1.hh connection0.hh query2.hh \
 coldata2.hh fields2.hh
row.o: row.cc result1.hh define_short field_names1.hh defs coldata1.hh \
 type_info1.hh const_string1.hh null1.hh row1.hh resiter1.hh \
 vallist1.hh manip1.hh sql_string1.hh sql_query1.hh sql_query0.hh \
 datetime1.hh stream2string1.hh tiny_int1.hh set1.hh tracker.h \
 field_types1.hh fields1.hh connection0.hh row3.hh row2.hh result2.hh \
 connection2.hh connection1.hh query1.hh query2.hh coldata2.hh \
 fields2.hh
set.o: set.cc set3.hh set2.hh set1.hh define_short coldata1.hh defs \
 type_info1.hh const_string1.hh null1.hh stream2string3.hh coldata2.hh
sql_query.o: sql_query.cc sql_query3.hh sql_query2.hh sql_query1.hh \
 define_short sql_query0.hh sql_string1.hh defs
type_info.o: type_info.cc datetime1.hh defs define_short coldata1.hh \
 type_info1.hh const_string1.hh null1.hh stream2string1.hh \
 tiny_int1.hh set1.hh type_info3.hh type_info2.hh
vallist.o: vallist.cc vallist3.hh vallist2.hh vallist1.hh manip1.hh \
 define_short sql_string1.hh defs sql_query1.hh sql_query0.hh \
 datetime1.hh coldata1.hh type_info1.hh const_string1.hh null1.hh \
 stream2string1.hh tiny_int1.hh set1.hh row2.hh row1.hh resiter1.hh \
 result2.hh connection2.hh connection1.hh query1.hh result1.hh \
 field_names1.hh tracker.h field_types1.hh fields1.hh connection0.hh \
 query2.hh coldata2.hh fields2.hh
