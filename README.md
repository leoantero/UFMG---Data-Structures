# Data Structures – UFMG

This repository contains the code developed during the **Data Structures** course at UFMG.

## 📦 Practical Assignments (TP)

### TP1 – Universal Sorter
Implements an abstract data type that selects the best sorting algorithm (insertion or quicksort) based on input characteristics and cost analysis. It calibrates parameters like partition size and disorder threshold empirically.

### TP2 – Logistics System Simulator
Discrete event simulation of a package delivery system between warehouses modeled as an undirected graph. Includes custom data types for packages, warehouses (with stacks), transport, and a priority queue scheduler.

### TP3 – Logistics Query System
Processes historical queries over package events. Supports:
- Package history by ID
- Package history by client
Indexed structures ensure fast access and response to real-time queries.

## 📘 Learning Activities (PA)

### PA01 – Sorting Algorithms Benchmark
Implements and compares various sorting algorithms: selection (iterative and recursive), insertion, quicksort (with variations), and shellsort. Tracks number of comparisons, moves, and function calls.

### PA02 – Min Heap
Implements a Min Heap data structure with insertion and removal, printing elements in sorted order.

### PA03 – Graphs
Receives graph input and performs operations like:
- Basic stats (number of vertices, edges, min/max degree)
- Printing neighborhoods
- Checking if the graph is complete

### PA04 – Disjoint Sets
Reads a graph and computes its minimum spanning tree using Union-Find (disjoint set structure). Requires tracking vertices, edges, and weights.

### PA05 – Dynamic Hash Table
Implements a set of strings using a dynamic hash table with linear rehashing. Supports:
- Insertion, deletion, membership test
- Set union, intersection, symmetric difference

## 🛠️ Technologies

- Language: C++
- Structure: `src/`, `include/`, `obj/`, `bin/`
- Compilation via `Makefile`

## 🚀 How to Compile

```bash
make all

