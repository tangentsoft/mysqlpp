#!/usr/bin/perl

$/ = undef;

open F, $ARGV[0];

$_=<F>;

s/ +/ /g;
s/\n /\n/g;
s/\n\n+/\n\n/g;

open F, ">$ARGV[0]";
print F;


