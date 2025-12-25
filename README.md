# Networking and System Projects

This repository contains a collection of projects demonstrating networking, system programming, and real-time communication concepts. These tools are designed for learners and developers who want to deepen their understanding of networking, socket programming, and system-level operations, particularly in Linux environments.

---

## 🛠️ Tools and Projects

### 1. 🛡️ ARP Spoof Detector
**Directory**: [`ARP_Sniffer`](https://github.com/Guruprasanth-M/socket/tree/master/ARP_Sniffer)  
- **Description**: A C-based ARP spoofing detection tool. Captures ARP packets, analyzes traffic, and identifies suspicious activities using threshold-based logic.  
- **Key Features**:
  - Live ARP packet sniffing with `libpcap`.
  - Real-time traffic analysis for ARP spoofing detection.
  - Threshold-based approach to flag bursts of abnormal ARP packets.
  - Designed for headless systems, such as servers and VMs.
- **How to Build and Use**:
  Use the `Makefile` provided in the directory to compile the project.  
  ```bash
  make
  sudo ./arp-spoof-detector -i eth0
  ```
- **Dependencies**:
  - `libpcap-dev` (Install with `sudo apt install libpcap-dev`)

---

### 2. 🧮 Math Server
**Directory**: [`math_server`](https://github.com/Guruprasanth-M/socket/tree/master/math_server)  
- **Description**: A Python server leveraging the Linux `bc` utility to provide advanced mathematical computations over TCP connections.
- **Key Features**:
  - Clients can connect and send mathematical queries for computation.
  - Supports arbitrary-precision calculations through `bc`.
  - Demonstrates Python's integration with system-level utilities.
- **How to Use**:
  Run the script within this directory and connect clients using utilities like `nc` or `telnet`.  
  Example client interaction:  
  ```bash
  nc localhost 3000
  >> 5 * (3 + 2)
  ```  

---

### 3. 💬 Multi-Client Chat Server
**Directory**: [`multithreaded_chat_server`](https://github.com/Guruprasanth-M/socket/tree/master/multithreaded_chat_server)  
- **Description**: A concurrent chat server built with Python's `socket` and `threading` modules.
- **Key Features**:
  - Real-time communication between multiple clients.
  - Broadcasts client messages to all users.
  - Detects and notifies when clients join or leave.
  - Thread-based architecture for handling multiple connections.
- **Source**: [`socket_1.py`](https://github.com/Guruprasanth-M/socket/blob/master/socket_1.py)

---

### 4. 🔄 Echo Server
**Directory**: [`echo_server`](https://github.com/Guruprasanth-M/socket/tree/master/echo_server)  
- **Description**: A basic server that echoes received messages back to clients.
- **Key Features**:
  - Demonstrates simple socket programming concepts.
  - Supports concurrent clients for bidirectional communication.
- **How to Use**:
  Start the server and connect clients using `telnet`.  

---

### 5. 🔧 Thread and Process Examples
**Directory**: [`thread_process`](https://github.com/Guruprasanth-M/socket/tree/master/thread_process)  
- **Description**: Demonstrates the usage of threads and multiprocessing in Python for concurrent tasks.
- **Key Features**:
  - Examples include creating threads and processes to handle specific tasks.
  - Shows how to share data safely between threads or processes.

---

### 6. 📦 Libpcap Utility
**Directory**: [`libpcap`](https://github.com/Guruprasanth-M/socket/tree/master/libpcap)  
- **Description**: Demonstrates how to utilize the `libpcap` library for packet sniffing. Explores capturing different network packets and analyzing them.
- **Key Features**:
  - Real-world example of using the `libpcap` library.
  - Foundation for building network monitoring applications in C.

---

### 7. 🌐 Client for Socket Server
**File**: [`client.py`](https://github.com/Guruprasanth-M/socket/blob/master/client.py)  
- **Description**: A Python-based TCP client that connects to server applications for testing and debugging.
- **Key Features**:
  - Can connect to any server running on a specified IP and port.
  - Serve as an interface for testing server responses.

---

## 🌍 Repository Structure

```plaintext
socket/
├── ARP_Sniffer/                # ARP Spoof Detector
├── math_server/                # Math server using bc
├── multithreaded_chat_server/  # Multi-client chat server
├── echo_server/                # Basic echo server
├── thread_process/             # Thread and process examples
├── libpcap/                    # Libpcap utility examples
├── client.py                   # TCP client for connecting to servers
├── socket_1.py                 # Python script for multithreaded chat server
└── README.md                   # Project documentation
```

---

## 💻 Requirements

- **Operating System**: Linux (recommended).
- **Languages**:
  - Python 3.x
  - C (with `libpcap`)
- **Dependencies**:
  - `libpcap-dev`: For packet capturing in C-based tools.

---

## 📦 Usage Examples

### ARP Spoof Detector:
```bash
make
sudo ./arp-spoof-detector -i eth0
```

### Multi-Client Chat Server:
1. Start the server:
   ```bash
   python socket_1.py
   ```
2. Connect clients using `telnet`:
   ```bash
   telnet localhost 4444
   ```

### Math Server:
Start the server and connect using `nc` or `telnet`:
```bash
nc localhost 3000
>> 5 + 5
```

---

## 📋 License

This repository is licensed under the [MIT License](https://github.com/Guruprasanth-M/socket/blob/master/README.md).

---

## ✍️ Author

- [Guruprasanth M](https://github.com/Guruprasanth-M)

---

## 🤝 Contributing

Contributions are welcome! Feel free to fork the repo and submit pull requests with improvements or additional tools.