import socket as s
import select as sel
import sys

HOST = ''
PORT = 4444
SOCKET_LIST = []
RECIVE_BUFFER = 4096

def chat_server():
    # creating a TCP socket AF_INET for internet and SOCK_STREAM for continuous data stream
    server_socket = s.socket(s.AF_INET,s.SOCK_STREAM)
    #setsockopt is used to set the scoket option and sol_socket is means which socket option to be set and so_reuseaddr is resue address
    server_socket.setsockopt(s.SOL_SOCKET,s.SO_REUSEADDR,1)
    # binding the socket to the host and port
    server_socket.bind((HOST,PORT))
    # its ready to start listening 
    server_socket.listen()
    # adding the server socket to the list of readable connections
    SOCKET_LIST.append(server_socket)

    print("Chat server started listening on port: "+str(PORT))
    
    while True:
        #blocking the flow for new incoming
        ready_read,ready_write,error = sel.select(SOCKET_LIST,[],[],0)
        
        for sock in ready_read:
            
            if sock == server_socket:
                client_socket,addr = sock.accept()
                SOCKET_LIST.append(client_socket)
                print("Client {}:{} connected".format(addr[0],addr[1]))
                broadcast(server_socket, client_socket,f"Client {format(addr)} entered the chatroom\n")

            else:
                try:
                    data = sock.recv(RECIVE_BUFFER)
                    if data:
                        data = data.decode()
                        broadcast(server_socket, sock,"[{}] {}".format(sock.getpeername(),data))
                    else:
                        # The socket is must have been broken remove it from the list
                        broadcast(server_socket, sock,"[{}] {}".format(sock.getpeername(),"Client is offline\n"))  
                        if sock in SOCKET_LIST:
                            SOCKET_LIST.remove(sock)
                except Exception as e:
                    broadcast(server_socket, sock,"[{}] {}".format(sock.getpeername(),"Client is offline\n"))  
                    continue
    #TODO: plan exit strategy       
    #server_socket.close()

def broadcast(server_socket,client_sock,message):
    print(f"Broadcast: {message}")
    for socket in SOCKET_LIST:
        if socket != server_socket and socket != client_sock:
            try:
                socket.send(message.encode())
            except:
                #it should be broken connection
                socket.close()
                if socket in SOCKET_LIST:
                    SOCKET_LIST.remove(socket)

if __name__ == "__main__":
    sys.exit(chat_server())