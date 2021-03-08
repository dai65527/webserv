# configファイル仕様書

## 文法
### 構造
config ファイルは、

- common context (nginxのhttp contextに対応)
- server context
- location context

なるものとする。

```
# common context

server {
    # server context
    
    location {
        # location context
    }
}

```

#### common context
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

#### 文法

```
server { <directives> }
```

#### default value
none

#### context
common

各サーバーディレクティブは最低でも1つ以上の以下のディレクティブを持たなければならない。

- listenディレクティブ
- rootディレクティブ


### locationディレクティブ
新しいlocation contextを作成する。一つのserver内に複数設定可能。ただし、同じpathを複数設定することはできない。

#### 文法
`<path>` にURLの絶対パスを1つ記述し、`{}` 内にlocationの設定を記述する。

```
listen <path> { <directives> }
```

#### 例

```
listen / {
    root /www/data;
}

listen /images/ {
    root /usr/images;
}
```

#### default value
none

#### context
server


### rootディレクティブ
URLの`/` がサーバー上のどのパスを示すかを絶対パスで1つ指定する。
1つのcontextにつき1つのみ設定できる。

#### 文法

```
root /www/var;
```

#### defauilt value
none

#### context
common, server, location


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
common, server, location


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
common, server, location


### charsetディレクティブ
リソースのcharsetを1つ指定することができる。
これを指定することによりwebservは以下を行う。

- リクエスト中に含まれる`Accept-Charset`ヘッダーの内容に合致しているか確認し、合致していない場合、`406 Not-Acceptable` を返す。
- レスポンスの`Content-Type` ヘッダ内の`charset` の内容を設定する。（ただし、CGIについては出力中に`Content-Type`ヘッダの記述が無い場合に限る）

以上は実際のリソースの文字コードに関係なく動作するため、リソースの文字コードと合致していない場合ブラウザ上の表示不具合を引き起こす可能性がある。

重複は不可。子contextで指定された場合は上書きする。

（nginxにない）

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
common, server, location


### languageディレクティブ
リソースの言語を1つ指定することができる。

これを指定することによりweb-servは以下を行う。
- リクエスト中に含まれる`Accept-Language`ヘッダーの内容に合致しているか確認し、合致していない場合、`406 Not-Acceptable` を返す。
- レスポンスの`Content-Type` 及び、`Content-Langage`ヘッダ内の`lang` の内容を設定する。（ただし、CGIについては出力中に`Content-Type`ヘッダの記述が無い場合に限る）

以上は実際のリソースの文字コードに関係なく動作するため、リソースの文字コードと合致していない場合ブラウザ上の表示不具合を引き起こす可能性がある。

重複は不可。子contextで指定された場合は上書きする。

（nginxにない）

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
common, server, location


### max_sessionsディレクティブ
このwebservで作成できる最大のセッション数を指定する
commonディレクティブに1回のみ記述できる

この上限を超える接続要求がソケットにきた場合は、`429 Too Many Requests` を返す。その際retry_afterディレクティブに指定された秒数を`Retry-After` ヘッダに指定する。

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
common


### retry_afterディレクティブ
`429 Too Many Requests` のレスポンスを返す際に、`Retry-After` ヘッダで指定する値を入力する。秒数で指定する。

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
common


### base_authディレクティブ
basic認証をonにする。onにするには`"`で囲まれた文字列を指定する。文字列はclientに表示される。

http://nginx.org/en/docs/http/ngx_http_auth_basic_module.html

#### 文法

```
base_auth: <string> | off
```

#### 例

```
base_auth: "Locked website"
base_auth: off
```

#### defauilt value
off

#### context
common, server, location


### auth_basic_user_fileディレクティブ
basic認証時に使用する`.htpasswd` ファイルのパスを指定する。（ファイルの内容については追記する）
複数指定できる

http://nginx.org/en/docs/http/ngx_http_auth_basic_module.html

#### 文法

```
base_auth_user_file: <path>
base_auth_user_file: <path> <path2>
```

#### 例

```
base_auth_user_file: /usr/.htpasswd
base_auth_user_file: /usr/.htpasswd /usr/share/.htpasswd
```

#### defauilt value
none

#### context
common, server, location