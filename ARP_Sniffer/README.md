🛡️ ARP Spoof Detector

A simple C-based ARP Spoofing detection tool for Linux systems.
This tool captures ARP packets from a specified network interface, analyzes traffic patterns, and detects suspicious activity (such as ARP spoofing) using a threshold-based approach.


🚀 Features

    Live ARP packet sniffing using libpcap

    Captures and inspects ARP packets in real time on a specified network interface

    Identifies ARP traffic based on Ethernet type (ETHERTYPE_ARP)

    Uses a threshold-based detection logic to identify suspicious ARP packet bursts

    Counts ARP packets within a fixed time window to flag abnormal behavior

    Resets detection counters after inactivity to avoid continuous false alerts

    Command-line interface with options to:

    display help

    list available network interfaces

    select a specific interface for sniffing

    display version information

    Runs as a pure command-line tool, suitable for servers, VMs, and headless systems

    Lightweight C implementation with minimal dependencies

---


## 🖥️ Dependencies

- libpcap-dev  
- Must run with root privileges  

### Install dependencies on Debian/Ubuntu

```markdown
```bash
sudo apt update
sudo apt install libpcap-dev


📂 Project Structure
arp-spoof-detector/
├── include/
│   └── arpsniffer.h
├── src/
│   ├── main.c
│   ├── sniff.c
│   ├── utils.c
│   └── ui.c
├── Makefile
└── README.md

