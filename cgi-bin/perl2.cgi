#!/usr/bin/perl
print "Content-Type: text/html\n\n";
print "<h1>Hello perl.cgi !!</h1>\n";
print "<p>AUTH_TYPE = \"$ENV{'AUTH_TYPE'}\"</p>\n";
print "<p>CONTENT_LENGTH = \"$ENV{'CONTENT_LENGTH'}\"</p>\n";
print "<p>CONTENT_TYPE = \"$ENV{'CONTENT_TYPE'}\"</p>\n";
print "<p>GATEWAY_INTERFACE = \"$ENV{'GATEWAY_INTERFACE'}\"</p>\n";
print "<p>PATH_INFO = \"$ENV{'PATH_INFO'}\"</p>\n";
print "<p>PATH_TRANSLATED = \"$ENV{'PATH_TRANSLATED'}\"</p>\n";
print "<p>QUERY_STRING = \"$ENV{'QUERY_STRING'}\"</p>\n";
print "<p>REMOTE_ADDR = \"$ENV{'REMOTE_ADDR'}\"</p>\n";
print "<p>REMOTE_IDENT = \"$ENV{'REMOTE_IDENT'}\"</p>\n";
print "<p>REMOTE_USER = \"$ENV{'REMOTE_USER'}\"</p>\n";
print "<p>REQUEST_METHOD = \"$ENV{'REQUEST_METHOD'}\"</p>\n";
print "<p>REQUEST_URI = \"$ENV{'REQUEST_URI'}\"</p>\n";
print "<p>SCRIPT_NAME = \"$ENV{'SCRIPT_NAME'}\"</p>\n";
print "<p>SERVER_NAME = \"$ENV{'SERVER_NAME'}\"</p>\n";
print "<p>SERVER_PORT = \"$ENV{'SERVER_PORT'}\"</p>\n";
print "<p>SERVER_PROTOCOL = \"$ENV{'SERVER_PROTOCOL'}\"</p>\n";
print "<p>SERVER_SOFTWARE = \"$ENV{'SERVER_SOFTWARE'}\"</p>\n";
print "\n";

#
# wwwperl.cgi?引数1+引数2 で指定したコマンド引数を書き出します。
#
print "=================================\n";
print "コマンド引数\n";
print "=================================\n";
for ($i = 0; $i <= $#ARGV; $i++) {
	print "ARGV[$i] = [ $ARGV[$i] ]\n";
}
print "\n";

#
# フォームに指定した値を読み込んで、書き出します。
#
print "=================================\n";
print "フォーム変数\n";
print "=================================\n";
if ($ENV{'REQUEST_METHOD'} eq "POST") {
	# POSTであれば標準入力から読込みます
	read(STDIN, $query_string, $ENV{'CONTENT_LENGTH'});
} else {
	# GETであれば環境変数から読込みます
	$query_string = $ENV{'QUERY_STRING'};
}
# 「変数名1=値1&変数名2=値2」の形式をアンパサンド( & )で分解します
@a = split(/&/, $query_string);
# それぞれの「変数名=値」について
foreach $a (@a) {
	# イコール( = )で分解します
	($name, $value) = split(/=/, $a);
	# + や %8A などをデコードします
	$value =~ tr/+/ /;
	$value =~ s/%([0-9a-fA-F][0-9a-fA-F])/pack("C", hex($1))/eg;
	# 変数名と値を書き出します
	print "$name = $value\n";
	# 後で使用する場合は、$FORM{'変数名'} に代入しておきます
	$FORM{$name} = $value;
}

#
# HTMLの終わりの部分を書き出します。
#
print "</XMP>\n";
print "</BODY>\n";
print "</HTML>\n";
print "\n";
