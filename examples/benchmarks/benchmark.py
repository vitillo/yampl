#!/usr/bin/env python

import sys
import os
import getopt
import math
import time
import thread

import yampl as ym


def run_async(func):
    """Async decorator (https://code.activestate.com/recipes/576684-simple-threading-decorator/)"""
    from threading import Thread
    from functools import wraps

    @wraps(func)
    def async_func(*args, **kwargs):
        func_hl = Thread(target=func, args=args, kwargs=kwargs)
        func_hl.start()
        return func_hl

    return async_func


def get_tid():
    return int(thread.get_ident()) * 31 % 3**10


def run_fork(func):
    """Fork decorator"""
    import os
    from functools import wraps

    @wraps(func)
    def fork_func(*args, **kwargs):
        pid = os.fork()

        if pid == 0:
            func(*args, **kwargs)
            sys.exit(0)
        else:
            return pid

    return fork_func


class Benchmark:
    """Class containing client/server benchmark asynchronous methods"""
    def __init__(self, channel, size, iterations, timeout=0):
        self.channel = channel
        self.size = int(size)
        self.iterations = int(iterations)
        self.mailbox = {}
        self.watermark = 'yampl'
        self.timeout = timeout

    @staticmethod
    def parse_context(context):
        context = context.lower()

        if context == 'thread':
            ctx = ym.Context.THREAD
        elif context == 'local_shm':
            ctx = ym.Context.LOCAL_SHM
        elif context == 'local_pipe':
            ctx = ym.Context.LOCAL_PIPE
        elif context == 'local':
            ctx = ym.Context.LOCAL
        elif context == 'distributed':
            ctx = ym.Context.DISTRIBUTED
        else:
            raise ValueError('Invalid context')

        return ctx

    @staticmethod
    def parse_context_s(context):
        if context == ym.Context.THREAD:
            ctx = 'thread'
        elif context == ym.Context.LOCAL_SHM:
            ctx = 'local_shm'
        elif context == ym.Context.LOCAL_PIPE:
            ctx = 'local_pipe'
        elif context == ym.Context.LOCAL:
            ctx = 'local'
        elif context == ym.Context.DISTRIBUTED:
            ctx = 'distributed'
        else:
            ctx = ''

        return ctx

    @staticmethod
    def parse_channel(channel):
        ctx = Benchmark.parse_context_s(channel.context)

        if channel.context == ym.Context.DISTRIBUTED:
            name = '127.0.0.1:3333'
        else:
            name = channel.name

        return name, ctx

    @staticmethod
    def make_payload(size, pattern):
        padding = int(size % len(pattern))
        payload = int(math.floor((size / len(pattern)))) * pattern + (padding * '\x00')
        return payload

    @run_fork
    def client_fork(self):
        self.client_thunk()

    @run_async
    def client_async(self):
        print('[+] Client launched on thread %d' % get_tid())
        self.client_thunk()

    def client_thunk(self):
        result = self.client()
        self.mailbox['client'] = result

    def client(self):
        channel_parsed = self.parse_channel(self.channel)
        socket = ym.ClientSocket(channel_parsed[0], channel_parsed[1])
        s_buffer = self.make_payload(self.size, self.watermark)

        start_tm = time.clock()

        for i in range(self.iterations):
            socket.send_raw(s_buffer)

            if self.channel.context == ym.Context.THREAD:
                size = -1

                while size == -1:
                    r_buffer = socket.try_recv_raw(self.timeout)
                    size = r_buffer[0]
            else:
                r_buffer = socket.recv_raw()

        end_tm = time.clock()

        return 1e6 * (end_tm - start_tm)

    @run_fork
    def server_fork(self, multiplicity):
        self.server_thunk(multiplicity)

    @run_async
    def server_async(self, multiplicity):
        print('[+] Server launched on thread %d' % get_tid())
        self.server_thunk(multiplicity)

    def server_thunk(self, multiplicity):
        result = self.server(multiplicity)
        self.mailbox['server'] = result

    def server(self, multiplicity):
        channel_parsed = self.parse_channel(self.channel)
        socket = ym.ServerSocket(channel_parsed[0], channel_parsed[1])
        s_buffer = self.make_payload(self.size, self.watermark)

        start_tm = time.clock()

        for i in range(self.iterations * multiplicity):
            if self.channel.context == ym.Context.THREAD:
                size = -1

                while size == -1:
                    r_buffer = socket.try_recv_raw(self.timeout)
                    size = r_buffer[0]
            else:
                r_buffer = socket.recv_raw()
            socket.send_raw(s_buffer)

        end_tm = time.clock()
        elapsed_tm = 1e6 * (end_tm - start_tm)

        return elapsed_tm


def main():
    sock_ctx = 'local_pipe'
    sock_channel = 'service'

    semantics = ym.Semantics.COPY_DATA  # MOVE_DATA is unsupported
    iterations = 1e3
    payload_size = 1e6
    multiplicity = 1

    opts, args = getopt.getopt(sys.argv[1:], 'n:s:c:m:')

    # Parse options
    for opt in opts:
        if opt[0] == '-n':
            iterations = int(opt[1])
        elif opt[0] == '-s':
            payload_size = int(opt[1])
        elif opt[0] == '-c':
            sock_ctx = opt[1]
        elif opt[0] == '-m':
            multiplicity = int(opt[1])

    print('[+] Running benchmark with: \n - Context: %s\n - Semantics: %s\n'
          ' - Iterations: %d \n - Payload Size: %d\n - Multiplicity: %d'
          % (sock_ctx.upper(), semantics, iterations, payload_size, multiplicity))

    # Run
    sock_channel_ = ym.Channel(sock_channel, Benchmark.parse_context(sock_ctx))
    ctxs = [ym.Context.LOCAL, ym.Context.LOCAL_PIPE, ym.Context.LOCAL_SHM, ym.Context.DISTRIBUTED]

    if sock_channel_.context in ctxs:
        benchmark = Benchmark(sock_channel_, payload_size, iterations)

        pids = []

        for i in range(0, multiplicity):
            pids.append(benchmark.client_fork())

        benchmark.server_thunk(multiplicity)

        for pid in pids:
            os.waitpid(pid, 0)
    elif sock_channel_.context == ym.Context.THREAD:
        """
        The THREAD context doesn't actually yield the expected results
        due to Python's internal multi-threading policy that implicates
        that only one thread can be holding the GIL (Global Interpreter
        Lock) at any one time. The result is highly degraded performance,
        unlock LOCAL, LOCAL_PIPE and DISTRIBUTED which run on their own 
        processes.
        """
        benchmark = Benchmark(sock_channel_, payload_size, iterations)

        benchmark.server_async(multiplicity)

        tasks = []

        for i in range(multiplicity):
            tasks.append(benchmark.client_async())

        for task in tasks:
            task.join()
    else:
        raise ValueError('Unimplemented context')

    server_tm = benchmark.mailbox['server']

    print('[+] Latency: %.0f uS' % (server_tm / (2 * iterations * multiplicity)))
    print('[+] Bandwidth: %.0f MB/s' % (2 * payload_size * iterations * multiplicity / server_tm))

    return


if __name__ == '__main__':
    try:
        main()
    except ValueError:
        print('Usage: python benchmark.py [-c context] [-n iterations] [-s size]')
