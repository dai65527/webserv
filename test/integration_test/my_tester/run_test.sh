#!/bin/bash
make re -C ../../../
../../../nginDX configfiles/test.conf&

sleep 1

ruby main.rb

if [ $? -ne 0 ]; then
    kill $!
    exit 1
else
    kill $!
fi
