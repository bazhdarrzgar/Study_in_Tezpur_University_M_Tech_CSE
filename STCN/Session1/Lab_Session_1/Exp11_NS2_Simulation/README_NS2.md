# Experiment 11: Simulation of Routing Algorithms using NS-2 Simulator

Course: **CSMT 502 — Selected Topics in Computer Networks (Tezpur University)**

---

## 1. Objective
To simulate and analyze the behavior of dynamic routing algorithms (specifically Distance Vector routing) under dynamic link failure and recovery scenarios using the Network Simulator 2 (NS-2).

---

## 2. Theory
Dynamic routing protocols allow routers to automatically adjust their forwarding paths when network topologies change due to router or link failures.

In NS-2:
- The Distance Vector algorithm is enabled using the command:
  ```tcl
  $ns rtproto DV
  ```
- Dynamic link failures and recoveries are scheduled using `rtmodel-at`:
  ```tcl
  $ns rtmodel-at 2.0 down $n1 $n4   ;# Bring link down at t = 2.0s
  $ns rtmodel-at 3.5 up   $n1 $n4   ;# Bring link back up at t = 3.5s
  ```

---

## 3. Network Topology
```
      (n0) -------- (n1) ================= (n4) -------- (n5) [Destination]
       |             |     (Fails at 2.0s)   |             |
       |             |     (Up at 3.5s)      |             |
      (n6) -------- (n2) ----------------- (n3) -----------+
```
- **Source Node**: `n0` (Traffic generated via CBR over UDP).
- **Destination Node**: `n5` (Traffic received via Null sink).
- **Primary Optimal Route**: `n0 -> n1 -> n4 -> n5` (Cost: 3 hops).
- **Alternate Route on Failure**: `n0 -> n1 -> n2 -> n3 -> n5` (Cost: 4 hops).

---

## 4. How to Run

### Step 1: Install NS-2 and NAM (if not already installed)
On Ubuntu / Debian:
```bash
sudo apt-get update
sudo apt-get install ns2 nam
```

### Step 2: Execute Simulation
```bash
ns routing_simulation.tcl
```
Or run the helper script:
```bash
bash run_ns2.sh
```

### Step 3: Visualize with Network Animator (NAM)
```bash
nam out.nam
```
- Click the **Play** button in NAM to observe packet flows.
- At $t = 2.0\text{ s}$, observe the link between `n1` and `n4` turn red/dashed (down). Packets will automatically redirect via `n2` and `n3`.
- At $t = 3.5\text{ s}$, observe the link turn black (up). Traffic promptly switches back to the shorter path `n1 -> n4`.

---

## 5. Trace File Format Analysis (`out.tr`)
Each line in `out.tr` represents a packet event:
```
<event> <time> <from_node> <to_node> <pkt_type> <pkt_size> <flags> <fid> <src_addr> <dst_addr> <seq_num> <pkt_id>
```
Where:
- `+`: Enqueue event
- `-`: Dequeue event
- `r`: Receive event
- `d`: Drop event (occurs when link goes down)
