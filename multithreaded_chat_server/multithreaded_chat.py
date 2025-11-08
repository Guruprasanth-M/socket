import socket
from threading import Thread

"""
Goals
----------
1.It has to be multithreaded
2.It has to be group chat server
3.one message has to be delivered to all clients
4.the server has to take user's nickname while connecting
5.tell when a user joins or leaves the chat
6.message length : 1024 bytes
"""

threads = []

class ChatserverOutgoingThread(Thread):
    def __init__()

class ChatServerIncomingThread(Thread):
    def __init__(self,conn,addr):
        Thread.__init__(self)
        self.conn = conn
        self.addr = addr
        self.nickname = ""
        self.user_ip = addr[0]
        self.user_port = addr[1]

    def setNickname(self,nick):
        self.nickname = nickname

    def getNickname(self):
        return self.nickname

    def isClosed(self):
        return self.conn._closed

    def sendMessage(self,message):
        fMessage = "{username} : {message}".format(username=self.nickname,message=message)
        self.conn.sendall(fMessage.encode())

    def run(self):


HOST = ''
PORT = 7777
binding = (HOST,PORT)

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM) 
                # create socket object af_in =ipv4, sock_stream = tcp 
s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1) 
                # to reuse the same address 
s.bind(binding) # bind the socket to the address
s.listen() # listen for connections

while not s._closed:
    conn, addr = s.accept() # accept connection from client
    #Move the accepted connection to a new thread

if not s._closed:
    s.close()