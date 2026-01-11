import socket
import time

HOST = '127.0.0.1'
PORT = 3030

# payloads to probe server protections
payloads = [
    "1+1\n",
    "2^20\n",
    "999999^999999\n",
    "99999!\n",
    "factor(999999999999999)\n",
]

while True:
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(3)
        s.connect((HOST, PORT))

        # read banner
        try:
            s.recv(4096)
        except:
            pass

        for p in payloads:
            try:
                print("> sending:", p.strip())
                s.sendall(p.encode())
            except BrokenPipeError:
                print("server dropped connection early")
                break

            # give server time to reply or kill
            time.sleep(0.1)

            try:
                out = s.recv(4096).decode()
                if out:
                    print(out)
            except:
                print("connection closed by server")
                break

        s.close()
        print("reconnecting...")
        time.sleep(0.2)

    except Exception as e:
        print("connect failed:", e)
        time.sleep(1)
