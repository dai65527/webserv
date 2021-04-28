require './WebservTest'

# テストケース例
casename = "get /"

## テスト名、ホスト、ポート、メソッドの指定
testcase = WebservTestCase.new casename, "localhost", 8888, "/", "GET"

## 期待されるレスポンスコード
testcase.expectedCode = "200"

## 期待されるボディの指定（指定なしなら触らない）
testcase.expectedBody = "<h1>/index.html</h1>\n"

## 期待されるレスポンスヘッダの指定（内容も確認）
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"

## 期待されるレスポンスヘッダの指定（存在のみ確認）
testcase.expectedResponseHeaderExitance.push "Date"
testcase.expectedResponseHeaderExitance.push "Last-Modified"

testcase.add

#####

casename = "get /index.html"
testcase = WebservTestCase.new casename, "localhost", 8888, "/", "GET"
testcase.expectedCode = "200"
testcase.expectedBody = "<h1>/index.html</h1>\n"
testcase.expectedResponseHeader["Server"] = "nginDX"
testcase.expectedResponseHeader["Content-Length"] = testcase.expectedBody.length.to_s
testcase.expectedResponseHeader["Connection"] = "keep-alive"
testcase.expectedResponseHeaderExitance.push "Date"
testcase.expectedResponseHeaderExitance.push "Last-Modified"
testcase.add

#####

exit(WebservTestCase::execAll)
