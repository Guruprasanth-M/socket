from subprocess import Popen, PIPE, STDOUT
p = Popen(['bc','-q','-i'],stdout=PIPE,stdin=PIPE,stderr=STDOUT)

#print(p.stdout.readline().rstrip())
result = p.communicate('1+2\n'.encode())
print("type = "+str(type(result)))
print(result[0].decode())
