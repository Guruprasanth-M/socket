import requests
import threading
import time
import signal
import sys
import random
from collections import deque, Counter, defaultdict

URL = "https://guruprasanth.selfmade.one"

USE_FAKE_HEADERS = True
THREADS = 16323
TARGET_RPS = 200         
WINDOW = 1
TIMEOUT = 5
SLOW_MS = 1000
WARMUP_SECONDS = 5

FAKE_HEADERS_POOL = [
    {"User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64)"},
    {"User-Agent": "Mozilla/5.0 (Linux; Android 10)"},
    {"User-Agent": "curl/8.4.0"},
    {"User-Agent": "python-requests/2.32.4"},
]

sent = deque()
done = deque()
lat = deque()
codes = Counter()
errors = Counter()
histogram = defaultdict(int)

lock = threading.Lock()
stop = threading.Event()

start_time = time.time()
tokens = TARGET_RPS
last_refill = time.time()

def now():
    return time.time()

def refill_tokens():
    global tokens, last_refill
    t = now()
    elapsed = t - last_refill
    tokens += elapsed * TARGET_RPS
    tokens = min(tokens, TARGET_RPS)
    last_refill = t

def take_token():
    global tokens
    while True:
        refill_tokens()
        if tokens >= 1:
            tokens -= 1
            return
        time.sleep(0.001)

def bucket(ms):
    if ms < 50: return "<50ms"
    if ms < 100: return "50-100ms"
    if ms < 250: return "100-250ms"
    if ms < 500: return "250-500ms"
    if ms < 1000: return "500-1000ms"
    return ">1000ms"

def worker(_):
    session = requests.Session()
    adapter = requests.adapters.HTTPAdapter(
        pool_connections=50,
        pool_maxsize=50,
        max_retries=0
    )
    session.mount("http://", adapter)
    session.mount("https://", adapter)

    while not stop.is_set():
        take_token()

        with lock:
            sent.append(now())

        headers = random.choice(FAKE_HEADERS_POOL) if USE_FAKE_HEADERS else None

        try:
            start = now()
            r = session.get(URL, headers=headers, timeout=TIMEOUT)
            ms = int((now() - start) * 1000)

            with lock:
                done.append(now())
                lat.append((now(), ms))
                codes[f"{r.status_code//100}xx"] += 1
                histogram[bucket(ms)] += 1

        except requests.exceptions.Timeout:
            with lock:
                errors["timeout"] += 1
        except requests.exceptions.ConnectionError:
            with lock:
                errors["conn"] += 1
        except Exception:
            with lock:
                errors["other"] += 1

def shutdown(sig, frame):
    print("\nStopping gracefully…")
    stop.set()
    time.sleep(0.5)
    sys.exit(0)

signal.signal(signal.SIGINT, shutdown)

for i in range(THREADS):
    threading.Thread(target=worker, args=(i,), daemon=True).start()

print("\nAdvanced SAFE load monitor started (Ctrl+C to stop)\n")

while True:
    time.sleep(1)
    t = now()

    with lock:
        while sent and t - sent[0] >= WINDOW:
            sent.popleft()

        while done and t - done[0] >= WINDOW:
            done.popleft()

        while lat and t - lat[0][0] >= WINDOW:
            lat.popleft()

        rps = len(sent)
        resp = len(done)
        times = [ms for _, ms in lat]

        if times:
            times.sort()
            avg = sum(times) / len(times)
            p95 = times[int(len(times) * 0.95) - 1]
            p99 = times[int(len(times) * 0.99) - 1]
        else:
            avg = p95 = p99 = 0

        err = sum(errors.values())

    state = "WARMUP" if (t - start_time) < WARMUP_SECONDS else (
        "STRESSED" if avg > SLOW_MS else "NORMAL"
    )

    print(
        f"\rRPS:{rps:<4} Resp/s:{resp:<4} "
        f"AVG:{int(avg):<4}ms P95:{p95:<4}ms "
        f"2xx:{codes['2xx']:<4} 4xx:{codes['4xx']:<3} 5xx:{codes['5xx']:<3} "
        f"Err:{err:<3} "
        f"{state}",
        end=""
    )
