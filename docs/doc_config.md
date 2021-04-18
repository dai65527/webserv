# configファイル仕様書

## 文法
### 構造
config ファイルは、

- main context (nginxのhttp contextに対応)
- server context
- location context

なるものとする。

```
# main context

server {
    # server context
    
    location {
        # location context
    }
}

```

#### main context
webserv全体の設定や、server context、location contextのデフォルトを設定する。

#### server context
ホスト、ポートを指定して、動作をserver contextごとに設定できるようにする。
バーチャルホストごとの設定なども含む。
nginxのserverディレクティブ内の設定とほぼ同等

#### location context
ポスト内であるURLパスのみ設定を変更したい場合に使用する。
nginxのlocationディレクティブないの設定とほぼ同等

### ブロック
server context、config contextは、
各ディレクティブ（`server`/`config`）に続く`{`と`}`の間に含まれる部分（ブロック）内に記述する。

```
server {
    # write config of server here
    location /path/ {
        # write config of location here
    }
}
```

### ディレクティブの設定
server、config以外のディレクティブについては、ディレクティブ名+スペースに続いて内容を書き、`;` までを設定内容とする。
複数記述する場合は、1つ以上のスペース（isspace(3)）で分割する。（改行も許可する）

```
server {
    directive_name setting1 setting2    setting3;
    anoter_directive_name setting1;
}
```

## ディレクティブ

### serverディレクティブ
新しいserver contextを作成する。一つのconfigファイルに複数設定可能。
server contextは続く`{`と`}`の中に記述する。
mainディレクティブにrootが設定されていない場合、
各サーバーディレクティブは最低でも1つrootディレクティブを持たなければならない。
また、各サーバーは一つ以上listenディレクティブを持たなければならない。

#### 文法

```
server { <directives> }
```

#### default value
none

#### context
main


### locationディレクティブ
新しいlocation contextを作成する。一つのserver内に複数設定可能。ただし、同じpathを複数設定することはできない。

#### 文法
`<path>` にURLの絶対パスを1つ記述し、`{}` 内にlocationの設定を記述する。

```
location <path> { <directives> }
```

#### 例

```
location / {
    root /www/data;
}

location /images/ {
    root /usr/images;
}
```

#### default value
none

#### context
server


### listenディレクティブ
接続要求を受け入れるホスト（またはIP）とポートを指定する。
ポート番号を省略した場合は80番ポートを使用する。
ホスト（またはIP）を省略した場合はホストアドレスを指定せず全てのIPアドレスへの接続要求を受け入れる。
ホスト（またはIP）には`*`を用いて全てのIPアドレスへの要求を受け入れることを示すことができる。
ホスト名の名前解決は`/etc/hosts`のみ使用する。

http://nginx.org/en/docs/http/ngx_http_core_module.html#listen

#### 文法

```
listen <IPv4 address | hostname>:<port number>;
listen <IPv4 address | hostname>;
listen <port number>;
```

#### 例

```
listen 80;
listen 42.42.42.42:8080;
listen localhost:8888;
listen localhost;
listen *:80;
```

#### default value
none

#### context
server


### rootディレクティブ
URLの`/` の指すディレクトリを絶対パスで1つ指定する。
1つのcontextにつき1つのみ設定できる。

#### 文法

```
root /www/var;
```

#### defauilt value
none

#### context
main, server, location


### server_name
仮想サーバーのホスト名を指定する。

#### 文法
1つのserverディレクティブ内で重複可能。

```
server_name <server_name>;
server_name <server_name1> <server_name2>;
```

#### 例

```
server_name www.example.com;
server_name example.com www.example.com;
```

#### default value
none

#### context
server


### autoindex
indexがないディレクトリにアクセスされた場合にディレクトリの内容を示すHTMLを返す。（ディレクトリリスティング）

#### 文法
`on`と`off`のみ指定できる。

```
autoindex < on | off >;
```

#### 例

```
autoindex on;
autoindex off;
```

#### default value
off

#### context
main, server, location


### error_page
HTTPエラーに対するデフォルトのエラーページを設定する

nginxと違い、`=`によるエラーメッセージの変更は定義しない。
http://nginx.org/en/docs/http/ngx_http_core_module.html#error_page

#### 文法

```
error_page <status_code> <path_to_error_page>
```

#### 例

```
error_page 404             /404.html;
error_page 500 502 503 504 /50x.html;
```

#### default value
none

#### context
main, server, context


### indexディレクティブ
URLでディレクトリが指定された際にデフォルトで表示するファイル名を指定する。
1つのcontext内で重複指定できる。
未指定の場合は親ディレクティブの設定を引き継ぐ。

#### 文法

```
index index.html index.php;
```

#### 例

```
index index.html index.php;
```

#### defauilt value
none

#### context
main, server, location


### cgi_extensionディレクティブ
cgiとして処理するファイルの拡張子を記述する
複数指定可能。
（nginxに無い）

#### 文法

```
cgi_extension <extensions without dot>;
```

#### 例

```
cgi_extension cgi rb; 
```

#### defauilt value
none

#### context
main, server, location


### charsetディレクティブ
リソースのcharsetを1つ指定することができる。
これを指定することによりwebservは以下を行う。

- リクエスト中に含まれる`Accept-Charset`ヘッダーの内容に合致しているか確認し、合致していない場合、`406 Not-Acceptable` を返す。
- レスポンスの`Content-Type` ヘッダ内の`charset`の内容を設定する。（ただし、CGIについては出力中に`Content-Type`ヘッダの記述が無い場合に限る）

以上は実際のリソースの文字コードに関係なく動作するため、リソースの文字コードと合致していない場合ブラウザ上の表示不具合を引き起こす可能性がある。

重複は不可。子contextで指定された場合は上書きする。

（nginxと一部異なる）
http://nginx.org/en/docs/http/ngx_http_charset_module.html#charset

#### 指定できるcharset
- utf-8
- 他（後々追加）

#### 文法

```
charset <charset>;
```

#### 例

```
charset utf-8;
```

#### defauilt value
none

#### context
main, server, location


### languageディレクティブ
リソースの言語を1つ指定することができる。

これを指定することによりweb-servは以下を行う。
- リクエスト中に含まれる`Accept-Language`ヘッダーの内容に合致しているか確認し、合致していない場合、`406 Not-Acceptable` を返す。
- レスポンスの`Content-Type` 及び、`Content-Langage`ヘッダ内の`lang` の内容を設定する。（ただし、CGIについては出力中に`Content-Type`ヘッダの記述が無い場合に限る）

重複は不可。子contextで指定された場合は上書きする。

https://www.nginx.com/resources/wiki/modules/accept_language/

#### 指定できるlanguage
（追記予定）

#### 文法

```
language <language>;
```

#### 例

```
language ja;
language ja-JP;
```

#### defauilt value
none

#### context
main, server, location


### max_sessionsディレクティブ
このwebservで作成できる最大のセッション数を指定する
mainディレクティブに1回のみ記述できる

この上限を超える接続要求がソケットにきた場合は、`503 Service Unavailable` を返す。その際retry_afterディレクティブに指定された秒数を`Retry-After` ヘッダに指定する。

(nginxにはない)

#### 文法

```
max_sessions <maximum number of sessions>;
```

#### 例

```
max_sessions 42;
```

#### defauilt value
none

#### context
main


### retry_afterディレクティブ
`503 Service Unavaileble`のレスポンスを返す際に、`Retry-After` ヘッダで指定する値を入力する。秒数で指定する。

(nginxにない)

#### 文法

```
retry_after <client time to wait to retry in sec>;
```

#### 例

```
retry_after 42;
```

#### defauilt value
42sec

#### context
main


### base_authディレクティブ
basic認証をonにする。onにするには`"`で囲まれた文字列を指定する。文字列はclientに表示される。
auth_basic_user_fileディレクティブと使用する。

http://nginx.org/en/docs/http/ngx_http_auth_basic_module.html

#### 文法

```
base_auth <string> | off;
```

#### 例

```
base_auth "Locked website";
base_auth off;
```

#### default value
off

#### context
main, server, location


### auth_basic_user_fileディレクティブ
basic認証時に使用する`.htpasswd` ファイルのパスを指定する。（ファイルの内容については追記する）
複数指定できる。
base_authディレクティブに指定する。

http://nginx.org/en/docs/http/ngx_http_auth_basic_module.html

#### 文法

```
base_auth_user_file <path>;
base_auth_user_file <path> <path2>;
```

#### 例

```
base_auth_user_file: /usr/.htpasswd;
base_auth_user_file: /usr/.htpasswd /usr/share/.htpasswd;
```

#### defauilt value
none

#### context
main, server, location


### client_max_body_size
クライアントから送られてくるリクエストボディの最大サイズを設定する。
単位はbyteで補助単位を（k, M, G）から選べる。大文字小文字は問わない。
値を0に設定すると、ボディサイズをチェックしない。
リクエストボディがこのサイズより大きい場合は413(Request Entity Too Large)を返す。

http://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size

#### 文法

```
client_max_body_size <size>;
```

#### 例

```
client_max_body_size 0;
client_max_body_size 42;
client_max_body_size 42k;
client_max_body_size 42K;
client_max_body_size 42m;
client_max_body_size 42M;
client_max_body_size 42g;
client_max_body_size 42G;
```

#### default value
1M

#### context
main
#### context
main, server, location


### upload_pass
ファイルをアップロードする際のパスを指定する。
このパスにPOSTメソッドを送ると、リクエストボディの内容に応じて、ファイルの追加が行えるようにする。
`upload_store` もセットで実装すること。

https://www.nginx.com/resources/wiki/modules/upload/

#### 文法

```
upload_pass <route>;
```

#### 例

```
upload_pass /upload;
```

#### default value
none

#### context
server, location


### upload_store
アップロードされたファイルを保存するディレクトリを指定する。
`upload_pass`とセットで使用すること。

#### 文法
ディレクトリを一つ指定する。

```
upload_store <ralative_path_from_root>;
```

#### 例

```
# file will be saved to /path/to/root/dir/path/to/upload
root /path/to/root/dir;
upload_store /path/to/upload;
```

#### default value
none

#### context
server, location


### limit_except

nginxのlimit_exceptと異なり、ブロックを取らず、全てのクライアントに対して指定されたメソッド以外を禁止する。
http://nginx.org/en/docs/http/ngx_http_core_module.html#limit_except

server、location、contextに記述された場合は、上位のディレクティブの設定を引き継がない。

#### 文法

```
limit_except <method1> <method2> <method3> ...;
```

#### 例

```
limit_except GET POST HEAD;
```

#### default value
GET, HEAD (暫定)

#### context
main, location, server
