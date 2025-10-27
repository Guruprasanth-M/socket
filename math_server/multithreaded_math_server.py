import socket
from subprocess import Popen, STDOUT, PIPE
from threading import Thread

HOST = ''
PORT = 6666

def start_math_server_thread(conn, addr):
    t = MathServerCommunicationThread(conn, addr)
    t.daemon = True
    t.start()

class ProcessOutputThread(Thread):
    def __init__(self, proc, conn):
        Thread.__init__(self)
        self.proc = proc
        self.conn = conn

    def run(self):
        while not self.proc.stdout.closed and not self.conn._closed:
            line = self.proc.stdout.readline()
            if not line:
                break
            self.conn.sendall(line)

class MathServerCommunicationThread(Thread):
    def __init__(self, conn, addr):
        Thread.__init__(self) #also this will work super().__init__()
        self.conn = conn
        self.addr = addr

    def run(self):
        print("{} connected with back port {}".format(self.addr[0], self.addr[1]))
        self.conn.sendall(
            b"Simple Math Server developed by LAHTP \n\nGive any math expressions, and I will answer you :) \n\n$ "
        )

        p = Popen(['bc'], stdout=PIPE, stdin=PIPE, stderr=STDOUT, shell=True)
        output = ProcessOutputThread(p, self.conn)
        output.daemon = True
        output.start()

        try:
            while True:
                data = self.conn.recv(1024)
                if not data:
                    break
                query = data.decode().strip()
                if query in ('quit', 'exit'):
                    p.stdin.write(b"quit\n")
                    p.stdin.flush()
                    p.wait(timeout=2)
                    break
                else:
                    p.stdin.write((query + '\n').encode())
                    p.stdin.flush()
        except Exception as e:
            print(f"Error: {e}")
        finally:
            try:
                self.conn.close()
            except Exception as e:
                print(f"Error closing connection: {e}")
            try:
                p.terminate()
            except Exception as e:
                print(f"Error terminating bc: {e}")
            print(f"Connection to {self.addr} closed.")

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen()
print(f"Math server listening on port {PORT}...")
try:
    while True:
        conn, addr = s.accept()
        start_math_server_thread(conn, addr)
except KeyboardInterrupt:
    print("Server shutting down.")
finally:
    s.close()