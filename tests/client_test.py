#!/usr/bin/env python

import yampl
import signal

signal.signal(signal.SIGINT, signal.SIG_DFL)

s = yampl.ClientSocket("service", "local_pipe")
s.send("hello world!")
size, buf = s.recv()
print(buf)
