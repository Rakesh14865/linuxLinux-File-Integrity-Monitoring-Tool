# Linux File Integrity Monitoring Tool (C++)

## Overview
This project is a Linux-based File Integrity Monitoring Tool developed in C++.  
It detects unauthorised file modifications, additions, and deletions by using cryptographic SHA-256 hashing and filesystem monitoring techniques. The tool automatically logs suspicious events with timestamps, making it useful for system security and forensic monitoring.

## Features
- Baseline file hash database creation
- Detection of:
  - Modified files
  - Newly created files
  - Deleted files
- Timestamped security logging
- Lightweight design suitable for embedded Linux environments
- Automated scheduled monitoring using cron

## Technologies Used
- C++
- Linux Filesystem APIs
- OpenSSL SHA-256 hashing
- Cron Scheduler
