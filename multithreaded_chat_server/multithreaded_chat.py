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

    def removeChatThread(self, thread):
        if thread in self.threads:
            self.threads.remove(thread)

    def queueMessage(self, user, message):
        data = (user, message)
        self.messages.append(data)

    def run(self):
        while True:
            sleep(0.025) #25ms
            # print(f"BOT message queue : {len(self.messages)}")
            # print(f"BOT thread length : {len(self.threads)}")
            if len(self.messages) > 0:
                for thread in self.threads:
                    for data in self.messages:
                        user = data[0]
                        msg  = data[1]
                        if thread.getusername() != user:
                            print(f"[BOT_send_message] {user}:{message}")
                            thread.sendMessage(msg)
                            self.message.remove(data)                            

                            
                            




class ChatserverOutgoingThread(Thread):
    def __init__(self, incoming_thread):
        Thread.__init__(self)
        self.incoming_thread = incoming_thread
        self.messages = []
        self.can_kill = False

    def sendMessage(self, user, message):
        # use the function argument `user`
        fMessage = f"{user} : {message}"
        try:
            conn = self.incoming_thread.getConnection()
            conn.sendall(fMessage.encode())
        except:
            bot.removeChatThread(self.incoming_thread)
            self.killThread()


    def queueMessage(self, user, message):
        data = (user, message)
        self.messages.append(data)
        print(f"BOT queue : {user}:{message}")

    def killThread(self, should_inform = False):
        #inform other that the client has disconnected.
        self.can_kill = True

    def run(self):
        while True:
            sleep(0.1) #100 milli sec
            if self.can_kill:
                break #if this loop breaks,the thread has to end.

            if len(self.messages) > 0:
                # IMPORTANT: take a copy and clear, or the same message
                # will be sent again and again forever
                current_messages = self.messages[:]
                self.messages.clear()

                for message in current_messages:
                    try:
                        print(f"[send_message] {message[0]}:{message[1]}")
                        self.sendMessage(message[0], message[1])
                        self.message.remove(message)
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
        self.outgoing_thread = None
        self.can_kill = False

    def setusername(self,username):
        self.username = username

    def getusername(self):
        return self.username

    def getConnection(self):
        return self.conn

    def isClosed(self):
        return self.conn._closed

    def initSendMessageThread(self):
        self.outgoing_thread = ChatserverOutgoingThread(self)
        self.outgoing_thread.start()

    def sendMessage(self,message):
        self.outgoing_thread.queueMessage(self.getUsername(),message)

    def broadcostMessage(self, message):
        self.outgoing_thread.queueMessage("Server BOT",message)

    def killThread(self):
        bot.removeChatThread(self)
        self.can_kill = True

    def run(self):
        self.initSendMessageThread()
        sleep(0.2)
        self.broadcostMessage("Welcome to the Group Chat server...\n")
        self.broadcostMessage(f"You're Connected from {self.user_ip}:{self.user_port}\n")
        self.broadcostMessage("Please enter your name to continue: ")
        name = self.conn.recv(1024)
        if not name:
            self.outgoing_thread.killThread()
            self.killThread()
            return
        else:
            self.setusername(name.decode()) 
        self.broadcostMessage("You can Chat with out Group...\n")   

        while not self.conn._closed:
            data = self.conn.recv(1024)
            if not data: #means client has disconnected.
                # inform other that the client has disconnected.
                self.outgoing_thread.killThread()
                self.killThread()
                break
            if data.decode().strip() == "kill":
                self.killThread()
            else:
                print("{ip}:{message}".format(ip = self.user_ip,message = data.decode()))
                bot.queueMessage(self.getusername(),data.decode().strip())




HOST = ''
PORT = 7777

bot = ChatBotThread()
bot.start()
binding = (HOST,PORT)

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM) 
                # create socket object af_in =ipv4, sock_stream = tcp 
s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1) 
                # to reuse the same address 
s.bind(binding) # bind the socket to the address
s.listen() # listen for connections
try:
    while not s._closed:
        conn, addr = s.accept() # accept connection from client
        #Move the accepted connection to a new thread
        t = ChatServerIncomingThread(conn,addr)
        t.start()
        bot.addChatThread(t)
except KeyboardInterrupt:
    print("\n[+] Server shutting down...")
    s.close()
    exit(0)


if not s._closed:
    s.close()
