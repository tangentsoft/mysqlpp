opendir DIR, "man-html";

foreach (readdir(DIR)) {
  next unless /(.+?)\.html$/;
  print "lynx -dump man-html/$_ > man-text/$1.txt\n" ;
  system "lynx -dump man-html/$_ > man-text/$1.txt" ;
}
