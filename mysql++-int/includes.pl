#!/usr/bin/perl

foreach $file (@ARGV) {
    @to_proc = ($file);
    %in_set = ();
    ($base) = $file =~ /(.+)\./;
    $objs .= " $base.o";

    for ($i = 0; $to_proc[$i]; $i++) {
	open IN, "$to_proc[$i]";
	$dir = "";
        ($dir) = $to_proc[$i] =~ /(.*?\/)/;
	while (<IN>) {
	    next unless /\#include *\"(.*?)\"/;
	    $incl = $dir  . $1;
	    push (@to_proc, $incl) unless $in_set{$incl};
	    $in_set{$incl} = 1;
	}
    }
    $rest.= "$base.o: ";
    foreach (@to_proc) {
	$rest.= "$_ ";
    }
    $rest.= "\n";
    $rest.= "\t \$(CC) \$(CPPFLAGS) \$(INCLUDE) -c $file\n";
    $rest.= "\n";
}
print << "---"
OBJS	= $objs

mysql++: \$(OBJS)
\t\$(CC) \$(LFLAGS) \$(LIBS) -o mysql++ \$(OBJS)

$rest

---


