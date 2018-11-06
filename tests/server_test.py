#!/usr/bin/env python

import yampl
import signal

signal.signal(signal.SIGINT, signal.SIG_DFL)

print('Creating ServerSocket...')
s = yampl.ServerSocket("service", "local_pipe")
print('ServerSocket created!')

size, buf = s.recv()
s.send("hello world!")
print(buf)
