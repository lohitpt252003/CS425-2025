### **README.md**
---

[Github link of the Assignment](https://github.com/lohitpt252003/CS425-2025.git)

## **Group members**
- LOHIT P TALAVAR (210564)
- ANJALI MALOTH (210146)

---

## **DNS Resolver - Iterative and Recursive Lookup**

A Python-based DNS resolver that supports **iterative** and **recursive** DNS lookups. Built with the `dnspython` library for handling DNS queries and responses.

---

## **Features**
1. **Iterative DNS Resolution**:
   - Follows the DNS hierarchy manually (Root → TLD → Authoritative servers).

2. **Recursive DNS Resolution**:
   - Relies on the system's default resolver to handle recursion.

3. **Error Handling**:
   - Handles timeouts, invalid domains, and unreachable servers.

4. **Command-line Interface**:
   - Simple command-line execution for ease of use.

---

## **Prerequisites**
- `dnspython` library

## **Installation**
1. Install the `dnspython` library:
   ```bash
   pip install dnspython
   ```
2. Clone/download the script `dns_resolver.py`.

---

## **Usage**

### **Command Syntax**
```bash
python3 dns_resolver.py <mode> <domain>
```
- **`<mode>`**: `iterative` or `recursive`
- **`<domain>`**: Domain name to resolve (e.g., `google.com`)

### **Examples**
1. **Iterative Lookup**:
   ```bash
   python3 dns_resolver.py iterative google.com
   ```

2. **Recursive Lookup**:
   ```bash
   python3 dns_resolver.py recursive example.com
   ```

---

## **Output Examples**

### **Iterative Lookup**
```plaintext
[Iterative DNS Lookup] Resolving google.com
[DEBUG] Querying ROOT server (198.41.0.4) - SUCCESS
Extracted NS hostname: a.gtld-servers.net.
Resolved a.gtld-servers.net. to 192.5.6.30
[DEBUG] Querying TLD server (192.5.6.30) - SUCCESS
...
[SUCCESS] google.com -> 142.250.194.78
Time taken: 0.597 seconds
```

### **Recursive Lookup**
```plaintext
[Recursive DNS Lookup] Resolving example.com
[SUCCESS] example.com -> ns1.example.com.
[SUCCESS] example.com -> 93.184.216.34
Time taken: 0.014 seconds
```

---

## **Error Handling**
- **Timeout**: If a server does not respond within 3 seconds.
- **NXDOMAIN**: If the domain does not exist.
- **Invalid Input**: Incorrect command-line arguments.

Example error output:
```plaintext
[ERROR] Query failed for ROOT server 198.41.0.4
[ERROR] Resolution failed.
```

---

## **Notes**
- The root servers are hardcoded in `ROOT_SERVERS` (only 5 are included for brevity).
- For recursive lookups, the system’s default resolver is used (e.g., `/etc/resolv.conf` on Unix).

---

## **Dependencies**
- `dnspython`: Used for DNS query construction and parsing.  
  Install with `pip install dnspython`.

---
