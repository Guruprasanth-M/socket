# Python Socket Projects

This repository contains Python projects demonstrating socket programming concepts, including networking, concurrency, and real-time communication. It is designed to help learners and developers understand practical socket usage in Linux environments.

## Projects

### 1. Multi-Client Chat Server

A concurrent TCP chat server built using Python's `socket` and `select` modules. Supports multiple clients, real-time message broadcasting, and robust connection management.

- Handles multiple clients simultaneously
- Broadcasts messages to all connected users
- Notifies users on client join/leave
- Gracefully manages disconnects and errors
- Foundation for learning event-driven networking

**Source:** [`socket_1.py`](socket_1.py)

### 2. Math Server using bc

A Python server that allows clients to connect and perform advanced mathematical operations. It leverages the Linux `bc` utility for arbitrary-precision calculations.

- Accepts math queries from clients over TCP
- Uses `bc` for accurate computation
- Supports complex arithmetic and expressions
- Real-time communication and result delivery
- Demonstrates Python and shell integration

## Requirements

- Python 3.x
- Linux (recommended for `bc` integration)
- No external Python dependencies

## Usage

**Run a server:**
```bash
python socket_1.py
```
_Replace with your project filename as needed._

**Connect as a client:**
You can use `telnet`, `nc`, or write your own Python client.

```bash
telnet localhost 4444
```
or
```bash
nc localhost 4444
```

For math server, connect and send math expressions; results will be returned.

## License

This repository is open source under the MIT License.

## Author

- [Guruprasanth M](https://github.com/Guruprasanth-M)

---

Feel free to clone, modify, or extend these examples for your own learning or projects!
