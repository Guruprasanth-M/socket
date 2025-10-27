import threading
from subprocess import Popen, PIPE

p = Popen(['bc','-i'], stdout=PIPE, stdin=PIPE, stderr=PIPE)

last_input = [None]  # Use list for mutability in the thread

def read_thread(p):
    while p.poll() is None:
        line = p.stdout.readline().decode()
        # Only print if it's NOT the last input sent to bc
        if last_input[0] is not None and line.strip() == last_input[0].strip():
            continue
        if line.strip() != "":
            print(line, end="")

t = threading.Thread(target=read_thread, args=(p,))
t.daemon = True
t.start()

while p.poll() is None:
    query = input()
    
    if query == "quit":
        p.terminate()
        break
    
    last_input[0] = query  # store input for filtering
    p.stdin.write((query + '\n').encode())
    p.stdin.flush()