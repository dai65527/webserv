#!/bin/bash
rm -f ./html/default_server/uploadstore/*.html
make re -C ../../../
../../../webserv configfiles/test.conf&

sleep 1

ruby main.rb

if [ $? -ne 0 ]; then
    kill $!
    exit 1
else
    kill $!
fi
