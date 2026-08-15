# PWAdmin 5.2 Free (Fixed Edition for Linux 64-bit / Ubuntu 22.04+)

This repository contains the fixed release of **PWAdmin 5.2 Free** by alexdnepro, patched to run stably on modern 64-bit Linux distributions (such as Ubuntu 20.04, 22.04, 24.04, Debian 11/12, CentOS 7/8/9).

---

## 🛠️ Issues Resolved in this Fixed Edition

### 1. Segmentation Fault on Client Connection (`getservbyname` / NSS Crash)
* **Problem**: When a client connects and authenticates, the original legacy 32-bit `pwadmin` binary called `getservbyname("mysql", "tcp")`. In modern 64-bit Linux with multiarch glibc (e.g. Ubuntu 22.04's `libc6:i386`), `getservbyname_r` dlopens dynamic `libnss_files.so.2` which calls `getrandom()` in dynamic `libc.so.6`. Because `pwadmin` is statically linked, dynamic TLS at `%gs:0x10` (VDSO entry point) was uninitialized, causing an immediate segmentation fault (`Addr: 0xF7F0703E`).
* **Fix**: The binary call site to `getservbyname` at offset `0x6ab97` and entry at `0x34c380` has been patched directly to return `NULL` (forcing default MySQL port 3306), completely eliminating the crashing NSS dlopen call.

### 2. First-Attempt Password Auto-Registration
* **Feature**: If `password = ` in `pwadmin.conf` is left blank, PWAdmin will automatically capture and register the exact hashed password sent by `pwadmin_free.exe` on the first client connection attempt.

### 3. NSS Service Resolution Fix
* On modern Linux, ensure `/etc/nsswitch.conf` contains:
  ```text
  services: files
  ```

---

## 🚀 Server Installation & Quick Start

1. **Copy server files**:
   ```bash
   cp -r server_side /pwadmin
   chmod -R 777 /pwadmin
   ```

2. **Configure `pwadmin.conf`**:
   * Set your MySQL database credentials:
     ```ini
     [Mysql]
     user = root
     pass = "YOUR_MYSQL_PASSWORD"
     host = 127.0.0.1
     port = 3306
     base = pw
     ```
   * Set `homepath = /home` (path to Perfect World server daemons).
   * Leave `password = ` blank for automatic first-time password registration.

3. **Start the daemon**:
   ```bash
   cd /pwadmin
   ./pwadmin start
   ```

4. **Stop the daemon**:
   ```bash
   cd /pwadmin
   ./pwadmin stop
   ```

5. **Logs**:
   Check runtime logs anytime with:
   ```bash
   tail -f /pwadmin/pwadmin_work.log
   ```

---

## 💻 Client Setup (Windows)

1. Extract `client_side` onto your Windows PC.
2. Run `pwadmin_free.exe`.
3. In the connection dialog:
   * **Host / IP**: Your Server IP
   * **Port**: `1500`
   * **Password**: Your preferred password (e.g. `pwadmin`)
4. Click **Connect**. The server will automatically register the password on first connection and open the management dashboard.
