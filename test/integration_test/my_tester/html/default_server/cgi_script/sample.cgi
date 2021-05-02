#!/usr/bin/perl

print STDERR "cgi script start\n";

print "Status: 777 Lucky\n";
print "Content-Type: text/plain; charset=utf-8\n";
print "Optional-Header: This is optional header\n";
print "\n";

print STDERR "cgi script fin write header\n";
print "CGI test\n";

for ($i = 0; $i <= $#ARGV; $i++) {
	print "ARGV[$i] = $ARGV[$i]\n";
}

read(STDIN, $stdinput, $ENV{'CONTENT_LENGTH'});
print "STDINPUT=$stdinput\n";

$query_string = $ENV{'QUERY_STRING'};
print "QUERYSTRING=$query_string\n";

print STDERR "cgi script end\n";
