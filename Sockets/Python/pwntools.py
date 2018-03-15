#!/usr/bin/env python2

# http://docs.pwntools.com/en/stable/tubes/sockets.html

from pwn import *

IP="127.0.0.1"
PORT=3000

r = remote(IP, PORT)
r.send("Hello")
print r.recv()
r.close()
