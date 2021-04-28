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
testcase.expectedResponseHeaderExitance.push "Date"
testcase.expectedResponseHeaderExitance.push "Last-Modified"

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
testcase.expectedResponseHeaderExitance.push "Date"
testcase.expectedResponseHeaderExitance.push "Last-Modified"
testcase.add

#####

casename = "get /nonexitstfile"
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/nonexitsfile", "GET"
testcase.expectedCode = "404"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExitance.push "Date"
testcase.expectedResponseHeaderExitance.push "Content-Length"
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
testcase.expectedResponseHeaderExitance.push "Date"
testcase.expectedResponseHeaderExitance.push "Last-Modified"
testcase.add

#####

casename = "get /dir_no_index (no index)"
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/dir_no_index", "GET"
testcase.expectedCode = "403"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExitance.push "Date"
testcase.expectedResponseHeaderExitance.push "Content-Length"
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
testcase.expectedResponseHeaderExitance.push "Date"
testcase.expectedResponseHeaderExitance.push "Last-Modified"
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
testcase.expectedResponseHeaderExitance.push "Date"
testcase.expectedResponseHeaderExitance.push "Last-Modified"
testcase.add

#####

casename = "get /autoindex_on/dir_no_index"
testcase = WebservTestCase.new casename, "127.0.0.1", 8000, "/autoindex_on/dir_no_index/", "GET"
testcase.expectedCode = "200"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeader["Content-Type"] = "text/html"
testcase.expectedResponseHeaderExitance.push "Date"
testcase.expectedResponseHeaderExitance.push "Content-Length"
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
testcase.expectedResponseHeaderExitance.push "Date"
testcase.expectedResponseHeaderExitance.push "Last-Modified"
testcase.add

#####

WebservTestCase::execAll
