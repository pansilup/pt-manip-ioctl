# pt-manip-ioctl

`pt-manip-ioctl` is a Linux kernel module that exposes an `ioctl()` interface to user space.  
It allows a user process to request **duplicate virtual address (VA) mappings (aliases)** for existing page frames.  
The alias VA range into which duplicates are created is **configurable in the kernel module code**.  

---

## Description

- The module installs an agent inside the kernel and provides an `ioctl()` entry point.  
- The **user provides existing virtual addresses** through the ioctl arguments.  
- For each supplied VA, the kernel agent looks up the corresponding page frame and inserts a **new alias mapping** for it in the **configured alias VA base range**.  
- This effectively creates multiple VA entries pointing to the same underlying physical memory.

---

## Usage

### 1. Include the header
User-space programs should include the kernel agent header:
```c
#include "kernel_agent.h"
