require "./WebservTest"

begin
  File.delete("html/default_server/uploadstore/upload.html")
rescue
  # nop
end

# テストケース例
casename = "get /"

## テスト名、ホスト、ポート、メソッドの指定
testcase = WebservTestCase.new casename, "localhost", 8000, "/", "GET"

## 期待されるレスポンスコード
testcase.expectedCode = "200"

## 期待されるボディの指定（指定なしなら触らない）
testcase.expectedBody = "<h1>/index.html</h1>\n"

## 期待されるレスポンスヘッダの指定（内容も確認）
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"

## 期待されるレスポンスヘッダの指定（存在のみ確認）
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"

testcase.add

#####

casename = "get /index.html"
testcase = WebservTestCase.new casename, "localhost", 8000, "/index.html", "GET"
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>/index.html</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "get /nonexitstfile"
testcase = WebservTestCase.new casename, "localhost", 8000, "/nonexitsfile", "GET"
testcase.expectedCode = "404"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Content-Length"
testcase.add

#####

casename = "get /directory (specify location)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/directory", "GET"
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>/dir1/index.html</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "get /dir_no_index (no index)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/dir_no_index", "GET"
testcase.expectedCode = "404"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Content-Length"
testcase.add

#####

casename = "get /dir_no_index/file1.txt"
testcase = WebservTestCase.new casename, "localhost", 8000, "/dir_no_index/file1.txt", "GET"
testcase.expectedCode = "200"
testcase.expectedBody = "testfile\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/plain"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "get /autoindex_on/ (but has index)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/autoindex_on/", "GET"
testcase.expectedCode = "200"
testcase.expectedBody = "\n"
testcase.expectedBody = "<h1>/index.html</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "get /autoindex_on/dir_no_index"
testcase = WebservTestCase.new casename, "localhost", 8000, "/autoindex_on/dir_no_index/", "GET"
testcase.expectedCode = "200"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Content-Length"
testcase.add

#####

casename = "get /autoindex_on/dir_no_index/file1.txt"
testcase = WebservTestCase.new casename, "localhost", 8000, "/autoindex_on/dir_no_index/file1.txt", "GET"
testcase.expectedCode = "200"
testcase.expectedBody = "testfile\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/plain"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "get /needauth/index.html with correct user pass"
testcase = WebservTestCase.new casename, "localhost", 8000, "/needauth/index.html", "GET"
testcase.request.basic_auth("user", "password")
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>Authorized!!</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "get /needauth/index.html without user pass"
testcase = WebservTestCase.new casename, "localhost", 8000, "/needauth/index.html", "GET"
testcase.expectedCode = "401"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeader["WWW-Authenticate"] = "Basic realm=\"Need Authentication\""
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Content-Length"
testcase.add

#####

casename = "get /needauth/index.html with invalid user"
testcase = WebservTestCase.new casename, "localhost", 8000, "/needauth/index.html", "GET"
testcase.request.basic_auth("hoge", "password")
testcase.expectedCode = "401"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeader["WWW-Authenticate"] = "Basic realm=\"Need Authentication\""
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Content-Length"
testcase.add

#####

casename = "get /needauth/index.html with invalid pass"
testcase = WebservTestCase.new casename, "localhost", 8000, "/needauth/index.html", "GET"
testcase.request.basic_auth("user", "hogehoge")
testcase.expectedCode = "401"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeader["WWW-Authenticate"] = "Basic realm=\"Need Authentication\""
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Content-Length"
testcase.add

#####

casename = "get /needauth/index.html with invalid user pass"
testcase = WebservTestCase.new casename, "localhost", 8000, "/needauth/index.html", "GET"
testcase.request.basic_auth("hoge", "hogehoge")
testcase.expectedCode = "401"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeader["WWW-Authenticate"] = "Basic realm=\"Need Authentication\""
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Content-Length"
testcase.add

#####

casename = "get /uploadstore/upload.html (file created in next case)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/uploadstore/upload.html", "GET"
testcase.expectedCode = "404"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Content-Length"
testcase.add

#####

casename = "put /upload/upload.html with body"
testcase = WebservTestCase.new casename, "localhost", 8000, "/upload/upload.html", "PUT"
testcase.request.body = "<h1>upload from test</h1>\n"
testcase.expectedCode = "201"
testcase.expectedBody = ""
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Location"] = "/uploadstore/upload.html"
testcase.expectedResponseHeader["Content-Location"] = "/uploadstore/upload.html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.add

#####

casename = "get /uploadstore/upload.html (file created in last case)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/uploadstore/upload.html", "GET"
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>upload from test</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "put /upload/upload.html with body (rewrite)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/upload/upload.html", "PUT"
testcase.request.body = "<h1>2nd upload</h1>\n"
testcase.expectedCode = "201"
testcase.expectedBody = ""
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Location"] = "/uploadstore/upload.html"
testcase.expectedResponseHeader["Content-Location"] = "/uploadstore/upload.html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Location"
testcase.add

#####

casename = "get /uploadstore/upload.html (file updated in last case)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/uploadstore/upload.html", "GET"
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>2nd upload</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"

#####

casename = "get /directory/youpi.bla (42 cgi tester)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/directory/youpi.bla", "GET"
testcase.expectedBody = ""
testcase.expectedCode = "200"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html; charset=utf-8"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.add

#####

casename = "get /cgi_script/sample.cgi (cgi script written in perl)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/cgi_script/sample.cgi", "GET"
testcase.expectedCode = "777"
testcase.expectedBody = "CGI test\nSTDINPUT=\nQUERYSTRING=\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/plain; charset=utf-8"
testcase.expectedResponseHeader["Optional-Header"] = "This is optional header"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.add

#####

casename = "get /cgi_script/sample.cgi/arg0/arg1/arg2/arg3 (cgi script written in perl with arguments)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/cgi_script/sample.cgi/arg0/arg1/arg2/arg3", "GET"
testcase.expectedCode = "777"
testcase.expectedBody = "CGI test\nARGV[0] = arg0\nARGV[1] = arg1\nARGV[2] = arg2\nARGV[3] = arg3\nSTDINPUT=\nQUERYSTRING=\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/plain; charset=utf-8"
testcase.expectedResponseHeader["Optional-Header"] = "This is optional header"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.add

#####

casename = "get /cgi_script/sample.cgi?arg0+arg1+arg2+arg3 (cgi script written in perl with arguments)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/cgi_script/sample.cgi?arg0+arg1+arg2+arg3", "GET"
testcase.expectedCode = "777"
testcase.expectedBody = "CGI test\nARGV[0] = arg0\nARGV[1] = arg1\nARGV[2] = arg2\nARGV[3] = arg3\nSTDINPUT=\nQUERYSTRING=arg0+arg1+arg2+arg3\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/plain; charset=utf-8"
testcase.expectedResponseHeader["Optional-Header"] = "This is optional header"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.add

#####

casename = "post /cgi_script/sample.cgi (cgi script written in perl)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/cgi_script/sample.cgi", "POST"
testcase.request.body = "this is test body"
testcase.expectedCode = "777"
testcase.expectedBody = "CGI test\nSTDINPUT=#{testcase.request.body}\nQUERYSTRING=\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/plain; charset=utf-8"
testcase.expectedResponseHeader["Optional-Header"] = "This is optional header"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.add

#####

casename = "post /cgi_script/sample.cgi/arg0/arg1/arg2/arg3 (cgi script written in perl)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/cgi_script/sample.cgi/arg0/arg1/arg2/arg3", "POST"
testcase.request.body = "this is test body"
testcase.expectedCode = "777"
testcase.expectedBody = "CGI test\nARGV[0] = arg0\nARGV[1] = arg1\nARGV[2] = arg2\nARGV[3] = arg3\nSTDINPUT=#{testcase.request.body}\nQUERYSTRING=\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/plain; charset=utf-8"
testcase.expectedResponseHeader["Optional-Header"] = "This is optional header"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.add

#####

casename = "post /cgi_script/sample.cgi?arg0+arg1+arg2+arg3 (cgi script written in perl)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/cgi_script/sample.cgi?arg0+arg1+arg2+arg3", "POST"
testcase.request.body = "this is test body"
testcase.expectedCode = "777"
testcase.expectedBody = "CGI test\nARGV[0] = arg0\nARGV[1] = arg1\nARGV[2] = arg2\nARGV[3] = arg3\nSTDINPUT=#{testcase.request.body}\nQUERYSTRING=arg0+arg1+arg2+arg3\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/plain; charset=utf-8"
testcase.expectedResponseHeader["Optional-Header"] = "This is optional header"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.add

#####

casename = "get /ファイル名.html in percent encoding"
testcase = WebservTestCase.new casename, "localhost", 8000, "/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB%E5%90%8D.html", "GET"
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>日本語でおk</h1>\n".force_encoding("ASCII-8BIT")  # need to change encoding to compare results
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "put /cgi_script/sample.cgi (cgi script written in perl)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/cgi_script/sample.cgi", "PUT"
testcase.request.body = "this is test body"
testcase.expectedCode = "777"
testcase.expectedBody = "CGI test\nSTDINPUT=#{testcase.request.body}\nQUERYSTRING=\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/plain; charset=utf-8"
testcase.expectedResponseHeader["Optional-Header"] = "This is optional header"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.add

#####

casename = "trace /index.html"
testcase = WebservTestCase.new casename, "localhost", 8000, "/index.html", "TRACE"
testcase.expectedCode = "200"
testcase.expectedBody = "TRACE /index.html HTTP/1.1\r\nAccept-Encoding: gzip;q=1.0,deflate;q=0.6,identity;q=0.3\r\nAccept: */*\r\nUser-Agent: Ruby\r\nHost: localhost:8000\r\n\r\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "message/http"
testcase.add

#####

casename = "head /index.html"
testcase = WebservTestCase.new casename, "localhost", 8000, "/index.html", "HEAD"
testcase.expectedCode = "200"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.add

#####

casename = "get /charset_utf8 with \"Accept-Charset: utf-8\" (has charset setting)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/charset_utf8", "GET"
testcase.request["Accept-Charset"] = "utf-8"
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>/index.html</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html; charset=utf-8"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "get /charset_utf8 with \"Accept-Charset: iso-8859-5\" (has charset setting, should return 406)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/charset_utf8", "GET"
testcase.request["Accept-Charset"] = "iso-8859-5"
testcase.expectedCode = "406"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.add

#####

casename = "get /language_en_US with \"Accept-Language: en-US\" (has charset setting)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/language_en_US", "GET"
testcase.request["Accept-Language"] = "en-US"
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>/index.html</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeader["Content-Language"] = "en-US"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "get /language_en_US with \"Accept-Language: en\" (has charset setting)"
testcase = WebservTestCase.new casename, "localhost", 8000, "/language_en_US", "GET"
testcase.request["Accept-Language"] = "en"
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>/index.html</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeader["Content-Language"] = "en-US"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "get /language_en_US with \"Accept-Language: ja-JP\" (has charset setting) should return 406"
testcase = WebservTestCase.new casename, "localhost", 8000, "/language_en_US", "GET"
testcase.request["Accept-Language"] = "ja-JP"
testcase.expectedCode = "406"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.add

#####

casename = "get anotherhost:8000/ (non default server should handle)"
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/", "GET"
testcase.request["Host"] = "anotherhost"
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>/non_default_server/index.html</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "get unknownhost:8000/ (default server should handle)"
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/", "GET"
testcase.request["Host"] = "unknownhost"
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>/index.html</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "get 127.0.0.1:8000/ (non default server should handle)"
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/", "GET"
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>/non_default_server/index.html</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

casename = "get localhost:8001/ (another port)"
testcase = WebservTestCase.new casename, "localhost", 8001, "/", "GET"
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>/non_default_server/index.html</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Last-Modified"
testcase.add

#####

WebservTestCase::execAll
