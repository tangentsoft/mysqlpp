
sub filter {
  local $_ = $_[0];
  s/\{\w*\//\0obrace-fsl\0/;
  s/([^\\])\{/$1\0obrace\0/;
  s/([^\\])\}/$1\0cbrace\0/;
  s/\\\{/\{/;
  s/\\\}/\}/;
  $_=&FILTER($_,"$tmpldir${X}other.flt");
  return $_;
}
