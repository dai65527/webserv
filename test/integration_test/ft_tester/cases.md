## Setup
### Make directories and files
Before starting please follow the next few steps (files content can be anything and will be shown to you by the test):
- Download the cgi_test executable on the host
- Create a directory YoupiBanane with:
        -a file name youpi.bad_extension
        -a file name youpi.bla
        -a sub directory called nop
                -a file name youpi.bad_extension in nop
                -a file name other.pouic in nop
        -a sub directory called Yeah
                -a file name not_happy.bad_extension in Yeah

### Configure
- / must answer to GET request ONLY
- /put_test/* must answer to PUT request and save files to a directory of your choice
- any file with .bla as extension must answer to POST request by calling the cgi_test executable
- /post_body must answer anything to POST request with a maxBody of 100
- /directory/ must answer to GET request and the root of it would be the repository YoupiBanane and if no file are requested, it should search for youpi.bad_extension files

### case1: GET /
Should return 200.

```
GET / HTTP/1.1
Host: localhost:8888
User-Agent: Go-http-client/1.1
Accept-Encoding: gzip
```

### case2: POST /
Should return 405.

```
POST / HTTP/1.1
Host: localhost:8888
User-Agent: Go-http-client/1.1
Transfer-Encoding: chunked
Content-Type: test/file
Accept-Encoding: gzip

0
```

### case3: HEAD /
Should return 405. (But nginx returns 200 when GET is allowed.)

```
HEAD / HTTP/1.1
Host: localhost:8888
User-Agent: Go-http-client/1.1
```

### case4: GET /directory (checking location and index directive)
Should return 200 and contents of `<root>/YoupiBanane/youpi.bad_extension`.

```
GET /directory HTTP/1.1
Host: localhost:8888
User-Agent: Go-http-client/1.1
Accept-Encoding: gzip
```

### case5: GET /YoupiBanene/youpi.bad_extension (checking location directive)
Should return 200 and contents of `<root>/YoupiBanane/youpi.bad_extension`.

```
GET /directory/youpi.bad_extension HTTP/1.1
Host: localhost:8888
User-Agent: Go-http-client/1.1
Accept-Encoding: gzip
```

### case6: GET /directory/youpi.bla (call CGI)
Should call cgi_tester executable (not cgi) and return 

```
GET /directory/youpi.bla HTTP/1.1
Host: localhost:8000
User-Agent: Go-http-client/1.1
Accept-Encoding: gzip
```

### case7: GET /directory/oulalala (file not found)
Should return 404.

```
GET /directory/oulalala HTTP/1.1
Host: localhost:8000
User-Agent: Go-http-client/1.1
Accept-Encoding: gzip
```

### case8: GET /directory/nop
Should return 200 and contents of `<root>/YoupiBanane/nop/youpi.bad_extension`.

```
GET /directory/nop HTTP/1.1
Host: localhost:8000
User-Agent: Go-http-client/1.1
Accept-Encoding: gzip
```

### case9: GET /directory/nop/
Should return 200 and contents of `<root>/YoupiBanane/nop/youpi.bad_extension`.

```
GET /directory/nop/ HTTP/1.1
Host: localhost:8000
User-Agent: Go-http-client/1.1
Accept-Encoding: gzip
```

### case10: GET /directory/nop/other.pouic
Should return 200 and contents of `<root>/YoupiBanane/nop/other.pouic`.

```
GET /directory/nop/other.pouic HTTP/1.1
Host: localhost:8000
User-Agent: Go-http-client/1.1
Accept-Encoding: gzip
```

### case11: GET /directory/nop/other.pouac (not found)
Should return 404.

```
GET /directory/nop/other.pouac HTTP/1.1
Host: localhost:8000
User-Agent: Go-http-client/1.1
Accept-Encoding: gzip
```

### case12: Test GET /directory/Yeah (is directory)
Should return 404.

```
GET /directory/Yeah HTTP/1.1
Host: localhost:8000
User-Agent: Go-http-client/1.1
Accept-Encoding: gzip
```

### case12: GET /directory/Yeah/not_happy.bad_extension
Should return 200 and content of 

```
GET /directory/Yeah/not_happy.bad_extension HTTP/1.1
Host: localhost:8000
User-Agent: Go-http-client/1.1
Accept-Encoding: gzip
```

### case13: PUT /put_test/file_should_exist_after
Should create file.

```
PUT /put_test/file_should_exist_after HTTP/1.1
Host: localhost:8000
User-Agent: Go-http-client/1.1
Transfer-Encoding: chunked
Accept-Encoding: gzip

3e8
eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
```
