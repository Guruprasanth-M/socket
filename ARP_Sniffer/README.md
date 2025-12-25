# 🛡️ ARP Spoof Detector

A simple C-based ARP Spoofing detection tool for Linux systems.

This tool captures ARP packets from a specified network interface, analyzes traffic patterns, and detects suspicious activity (such as ARP spoofing) using a threshold-based approach.

---

## 🚀 Features

- Live ARP packet sniffing using `libpcap`.
- Captures and inspects ARP packets in real time from a selected network interface.
- Identifies ARP traffic based on Ethernet type (`ETHERTYPE_ARP`).
- Threshold-based detection logic to identify suspicious ARP packet bursts.
- Resets detection counters after inactivity to avoid continuous false alerts.
- Command-line interface (CLI) with options to:
  - Display help
  - List available network interfaces
  - Select a specific interface for sniffing
  - Display version information
- Lightweight C implementation with minimal dependencies.
- Ideal for servers, VMs, and headless systems.

---

## 🖥️ Dependencies

This tool requires the following dependencies:
- `libpcap-dev`

### Install dependencies on Debian/Ubuntu

```bash
sudo apt update
sudo apt install libpcap-dev
```

---

## 📦 Usage

**Note:** This tool requires root privileges to capture ARP packets from network interfaces.

### Build the Tool

Compile the project using the provided Makefile:

```bash
make
```

### Run the Tool

After building, execute the binary with appropriate options:

```bash
sudo ./arp-spoof-detector --help
sudo ./arp-spoof-detector -i eth0
```

#### Command-Line Options

- `--help`: Display the help menu.
- `--list-interfaces`: List available network interfaces.
- `-i [interface]`: Specify the interface to start sniffing.
- `--version`: Display version information.

---

## 📂 Project Structure

```plaintext
arp-spoof-detector/
├── include/
│   └── arpsniffer.h             # Header files for ARP sniffer functions
├── src/
│   ├── main.c                   # Entry point of the application
│   ├── sniff.c                  # Handles live ARP packet sniffing and analysis
│   ├── utils.c                  # Utility functions for processing ARP packets
│   └── ui.c                     # CLI and user interface logic
├── Makefile                     # Build configuration for the project
└── README.md                    # Project documentation
```

---

## 📖 Example Output

On detecting ARP spoofing, the tool will output alerts to the terminal in real-time:

```plaintext
Suspicious activity detected: ARP traffic burst from [xx:xx:xx:xx:xx:xx] on eth0!
```

---

## 👷 Future Improvements

- [ ] Add logging support for detected spoofing incidents.
- [ ] Create a test suite for the project.
- [ ] Implement email notifications for detected incidents.

---

## 🤝 Contributing

Contributions are welcome! Please fork the repository and submit a pull request.