$/ = undef;

open F, $ARGV[0];

$_=<F>;

s~\\begin_inset LatexCommand \\ref.+
\\end_inset
(.+)
\\begin_inset LatexDel .+
\\end_inset
.+
\\begin_inset LatexDel .+
\\end_inset
~\\begin_inset LatexCommand \\ref\{$1\}

\\end_inset
~g;

open F, ">$ARGV[0]";
print F $_;

