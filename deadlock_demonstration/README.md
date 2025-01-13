# Deadlock Demonstration in C

This repository contains a C program that demonstrates the concept of **deadlock** and provides solutions to avoid it using two techniques: **lock ordering** and **trylock**. The program runs on Ubuntu-based Linux systems.

---

## What is a Deadlock?

A **deadlock** occurs in concurrent programming when two or more threads are waiting indefinitely for each other to release resources, preventing further execution. It typically happens when:
1. Threads acquire locks in different orders.
2. Circular waiting occurs between threads for a set of resources.

For example, if:
- Thread 1 holds `Lock A` and waits for `Lock B`.
- Thread 2 holds `Lock B` and waits for `Lock A`.

Neither thread can proceed, resulting in a **deadlock**.

---

## Program Description

This program demonstrates:
1. **Deadlock Problem**:
   - Threads acquire locks in an inconsistent order, resulting in a deadlock.
   - Example: 
     - Thread 1 acquires `lock1` and waits for `lock2`.
     - Thread 2 acquires `lock2` and waits for `lock1`.

2. **Lock Ordering Solution**:
   - Threads acquire locks in a consistent predefined order (e.g., always acquire `lock1` first, then `lock2`).
   - This eliminates circular waiting, thus preventing deadlock.

3. **Trylock Solution**:
   - Threads attempt to acquire locks using `pthread_mutex_trylock`.
   - If a lock is unavailable, they release already acquired locks and retry after a short delay.
   - This avoids indefinite blocking and potential deadlocks.

---

## How to Run the Program

### Prerequisites
- A Linux-based system (e.g., Ubuntu).
- GCC compiler for compiling the program.

### Steps to Compile and Run
1. Clone this repository and navigate to the directory containing `deadlock_demonstration.c`.
2. Compile the program using the following command:
   ```bash
   gcc -pthread deadlock_demonstration.c -o deadlock_demonstration

Run the program using:
 ```bash
./deadlock_demonstration


