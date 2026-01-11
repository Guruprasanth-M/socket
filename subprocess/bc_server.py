from threading import Thread
from subprocess import Popen, PIPE, STDOUT

class ProcessOutputThread(Thread):
    def __init__(self,p):
        super().__init__()
        self.p = p
    def run(self):
        while self.p.poll() is None:
            line = self.p.stdout.readline()
            if line:
                print(f"Output: {line.strip()}")

p = Popen(['bc', '-q'], stdin=PIPE, stdout=PIPE, stderr=STDOUT, text=True)

out_t = ProcessOutputThread(p)
out_t.start()
while p.poll() is None:
    inp = input(" ")
    inp = inp + "\n"
    p.stdin.write(inp)
    p.stdin.flush()