$/ = undef;

open F, "manual.tex";
open CR, "../sqlplusint/doc.tex";

$_=<F>;
$classref=<CR>;

s~\\begin{comment}[\s\n]*Begin Class Ref[\s\n]*\\end{comment}.+?\\begin{comment}[\s\n]*End Class Ref[\s\n]*\\end{comment}~
\\begin{comment}
Begin Class Ref
\\end{comment}
$classref
\\begin{comment}
End Class Ref
\\end{comment}
~s || die "Could not insert classref\n";

open F, ">manual.tex";
print F $_; 


