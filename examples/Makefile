
include ../Configure

CFLAGS += -O0
EXAMPLES = simple1 reset-db custom1 custom2 custom3 custom4 complic1 fieldinf1

examples: $(EXAMPLES)

simple1: simple1.o

simple1.o: simple1.cc


complic1: complic1.o

complic1.o: complic1.cc


fieldinf1: fieldinf1.o

fieldinf1.o: fieldinf1.cc


custom1: custom1.o

custom1.o: custom1.cc


custom2: custom2.o util.o

custom2.o: custom2.cc util.hh


custom3: custom3.o util.o

custom3.o: custom3.cc util.hh


custom4: custom4.o

custom4.o: custom4.cc


util.o: util.cc util.hh


reset-db: reset-db.o

reset-db.o: reset-db.cc

clean: 
	-rm -f $(EXAMPLES)
