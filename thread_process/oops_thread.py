import threading
from subprocess import Popen, PIPE

p = Popen(['bc','-i'], stdout=PIPE, stdin=PIPE, stderr=PIPE)
last_input = [None]  # Shared between main loop and thread

class ProcessOutputThread(threading.Thread):
    def __init__(self, process, last_input):
        super().__init__()
        self.process = process
        self.last_input = last_input
    
    def run(self):
        while self.process.poll() is None:
            line = self.process.stdout.readline().decode()
            # Only print if it's NOT the last input sent to bc
            if self.last_input[0] is not None and line.strip() == self.last_input[0].strip():
                continue
            if line.strip() != "":
                print(line, end="")

t = ProcessOutputThread(p, last_input)
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