import socket
from threading import Thread
from time import sleep

"""
Goals
----------
1.It has to be multithreaded
2.It has to be group chat server
3.one message has to be delivered to all clients
4.the server has to take user's username while connecting
5.tell when a user joins or leaves the chat
6.message length : 1024 bytes
"""



class ChatBotThread(Thread):
    def __init__(self):
        Thread.__init__(self)
        self.threads = []
        self.messages = []

    def addChatThread(self, thread):
        self.threads.append(thread)

    def queueMessage(self,user,message):
        fMessage = "{username} : {message}".format(username = user , message = message)
        data = (user,fMessage)
        self.messages.append(fMessage)

    def run(self):
        while True:
            sleep(0.025) #25ms
            if len(self.messages) > 0:
                for thread in self.threads:
                    for message in self.messages:
                        if thread.getusername() != message[0]:
                            #to be contiue start for 2.1hr
                            pass




class ChatserverOutgoingThread(Thread):
    def __init__(self,incoming_thread):
        Thread.__init__(self)
        self.incoming_thread = incoming_thread
        self.messages = []
        self.can_kill = False

    def sendMessage(self,message):
        fMessage = f"{self.username} : {message}"
        try:
            conn = self.incoming_thread.getConnection()
            conn.sendall(fMessage.encode())
        except:
            self.killThread()

    def queueMessage(self,message):
        self.messages.append(message)

    def killThread(self,should_inform = False):
        self.can_kill = True

    def run(self):
        while True:
            sleep(0.1) #100 milli sec
            if self.can_kill:
                break #if this loop breaks,the thread has to end.
            if len(self.messages) > 0:
                for message in self.messages:
                    try:
                        self.sendMessage(message);
                    except:
                        #inform other that the client has disconnected.
                        break




class ChatServerIncomingThread(Thread):
    def __init__(self,conn,addr):
        Thread.__init__(self)
        self.conn = conn
        self.addr = addr
        self.username = ""
        self.user_ip = addr[0]
        self.user_port = addr[1]
        self.incoming_thread = None
        self.can_kill = False

    def setusername(self,username):
        self.username = username

    def getusername(self):
        return self.username

    def getConnection(self):
        return self.conn

    def isClosed(self):
        return self.conn._closed

    def initSendMessageThread(self,message):
        self.incoming_thread = ChatserverOutgoingThread(self)

    def killThread(self):
        self.can_kill = True

    def run(self):
        while self.conn._closed:
            data = self.conn.recv(1024)
            if not data: #means client has disconnected.
                # inform other that the client has disconnected.
                self.incoming_thread.killThread()
                break







HOST = ''
PORT = 7777

bot = ChatBotThread()

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
    t = ChatServerIncomingThread(conn,addr)
    t.start()
    bot.addChatThread(t)

if not s._closed:
    s.close()