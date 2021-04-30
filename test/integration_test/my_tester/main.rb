require './WebservTest'

# テストケース例
casename = "get /"

## テスト名、ホスト、ポート、メソッドの指定
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/", "GET"

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
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/index.html", "GET"
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
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/nonexitsfile", "GET"
testcase.expectedCode = "404"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Content-Length"
testcase.add

#####

casename = "get /directory (specify location)"
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/directory", "GET"
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
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/dir_no_index", "GET"
testcase.expectedCode = "403"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Content-Length"
testcase.add

#####

casename = "get /dir_no_index/file1.txt"
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/dir_no_index/file1.txt", "GET"
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
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/autoindex_on/", "GET"
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
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/autoindex_on/dir_no_index/", "GET"
testcase.expectedCode = "200"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Content-Length"
testcase.add

#####

casename = "get /autoindex_on/dir_no_index/file1.txt"
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/autoindex_on/dir_no_index/file1.txt", "GET"
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
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/needauth/index.html", "GET"
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
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/needauth/index.html", "GET"
testcase.expectedCode = "401"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeader["WWW-Authenticate"] = "Basic realm=\"Need Authentication\""
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Content-Length"
testcase.add

#####

casename = "get /needauth/index.html with invalid user"
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/needauth/index.html", "GET"
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
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/needauth/index.html", "GET"
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
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/needauth/index.html", "GET"
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
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/uploadstore/upload.html", "GET"
testcase.expectedCode = "404"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Content-Length"
testcase.add

#####

casename = "put /upload/upload.html with body"
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/upload/upload.html", "PUT"
testcase.request.body = "<h1>upload from test</h1>\n"
testcase.expectedCode = "201"
testcase.expectedBody = ""
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Location"] = "/uploadstore/upload.html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Location"
testcase.add

#####

casename = "get /uploadstore/upload.html (file created in last case)"
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/uploadstore/upload.html", "GET"
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
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/upload/upload.html", "PUT"
testcase.request.body = "<h1>2nd upload</h1>\n"
testcase.expectedCode = "201"
testcase.expectedBody = ""
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Location"] = "/uploadstore/upload.html"
testcase.expectedResponseHeaderExistance.push "Date"
testcase.expectedResponseHeaderExistance.push "Location"
testcase.add

#####

WebservTestCase::execAll
