
$/ = undef;
open IN, "manual.tex";
$_ = <IN>;

s/\\begin\{comment\}[\n\s]+example:(\S+)[\n\s]*\\end\{comment\}[\n\s]*/${\(&ie($1))}/sg;

open IN, ">manual.tex";
print IN;

sub ie {
  my $return;
  $return  = "\n";
  $return .= "\\begin{verbatim}\n";
  open EIN, "examples/$_[0]";
  local $/ = undef;
  $return .= <EIN>;
  $return .= "\\end{verbatim}\n\n";
  return $return;
}
