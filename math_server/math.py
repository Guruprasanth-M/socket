from subprocess import Popen, PIPE, STDOUT
from threading import Thread
class ProcessOutputThread(Thread):
    def __init__(self,proc):
        Thread.__init__(self)
        self.proc = proc
    
    def run(self):
        while not self.proc.stdout.closed:
                print(self.proc.stdout.readline().rstrip().decode())

p = Popen(['bc','-q','-i'],stdout=PIPE,stdin=PIPE,stderr=STDOUT,shell=True)
Output = ProcessOutputThread(p)
Output.start()

while not p.stdout.closed:
    query = input(">>")
    if query == "quit":
        p.communicate(query.encode(),timeout=1)
        if p.poll() is not None:
            break
    query = query + "\n"
    p.stdin.write(query.encode())
    p.stdin.flush()
