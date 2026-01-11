import socket
from subprocess import Popen,STDOUT,PIPE
from threading import Thread

HOST=''
PORT=3030
con = []

class ProcessOutputThread(Thread):
    def __init__(self,proc,conn,addr):
        Thread.__init__(self)
        self.proc = proc
        self.conn = conn
        self.addr = addr

    def run(self):
            while self.proc.poll() is None:
                try:
                    self.conn.sendall(self.proc.stdout.readline())
                except:
                    pass

class mathserverthread(Thread):
    def __init__(self,conn,addr):
     Thread.__init__(self)
     self.conn=conn
     self.addr=addr

    def run(self):
        p = Popen(['bc'],stdout=PIPE,stderr=STDOUT,stdin=PIPE)
        out_t = ProcessOutputThread(p,self.conn,self.addr)
        out_t.start()
        while p.poll() is None:
            try:
                inp = self.conn.recv(1024)
                inp = inp.decode().strip()
                if not inp:
                    break
                else:
                    if inp == 'exit' or inp == 'quit':
                        p.communicate(inp.encode(), timeout=1)
                        if p.poll() is not None:
                            con.remove(addr[0])
                            self.conn.close() 
                            break                            
                    inp = inp + '\n'
                    p.stdin.write(inp.encode())
                    p.stdin.flush()    
            except Exception as e:
                print(addr[0]+"-"+e)
                con.remove(addr[0]) 
                self.conn.close()
                
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
s.bind((HOST,PORT))
s.listen()
while True:
    conn,addr = s.accept()
    # limit of IP Address
    if con.count(addr[0])==3 :
        print("connection rejected from {}:{}".format(addr[0],addr[1]))
        conn.close()
    else:
        con.append(addr[0])
        print("connection Accept from {}:{}".format(addr[0],addr[1]))
        t= mathserverthread(conn,addr)
        t.start()

