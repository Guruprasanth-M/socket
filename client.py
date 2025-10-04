import sys
import socket
import select as sel

SOCKET_LIST = [sys.stdin]

def chat_client():
    if(len(sys.argv) < 3):
        print("Usage : python3 {} hostname port".format(sys.argv[0]))
        sys.exit(1)
    host = sys.argv[1]
    port = int(sys.argv[2])

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(5)

    try:
        s.connect((host, port))
    except:
        print("Cannot reach the {} : {}".format(host, port))
        sys.exit(-1)
    print("Connected to remote host. You can start sending messages")
    sys.stdout.write('> ')
    sys.stdout.flush()

    while True:
        ready_read, ready_write, error = sel.select([s, sys.stdin], [], [])
        for sock in ready_read:
            if sock == s:
                data = s.recv(4096)
                if not data:
                    print("\nDisconnected from chat server")
                    sys.exit()
                else:
                    sys.stdout.write(data.decode())
                    sys.stdout.write('> ')
                    sys.stdout.flush()
            else:
                msg = sys.stdin.readline()
                s.send(msg.encode())
                sys.stdout.write('> ')
                sys.stdout.flush()


if __name__ == "__main__":
    sys.exit(chat_client())