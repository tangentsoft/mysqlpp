$/ = undef;

open F, $ARGV[0];

$_=<F>;

/(\\layout Section.+)\\the_end/s;

open F, ">$ARGV[0]";
print F $1;

