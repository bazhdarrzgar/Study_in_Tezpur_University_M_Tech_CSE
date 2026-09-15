# Tezpur University
## Sessional Test-1 — Autumn 2026
**Course Code:** CSMT 502  
**Course Title:** Selected Topics in Computer Networks  
**Max. Marks:** 20 M | **Time:** 20 Min  

---

## Quick Answer Key

| Q. No. | Section | Question Topic | Correct Answer |
|:---:|:---|:---|:---|
| **1** | I. MCQ | OSI Data Link vs Network Layer Responsibilities | **B. Data Link: hop-to-hop; Network: source-to-destination** |
| **2** | I. MCQ | Out-of-order & different routes switching | **C. Datagram packet switching** |
| **3** | I. MCQ | Real-time application delay & jitter evaluation | **B. X may provide smoother delivery because its delay variation is substantially smaller** |
| **4** | I. MCQ | Distinguishing throughput from bandwidth | **C. Bandwidth represents theoretical capacity, whereas throughput represents the actual achieved rate.** |
| **5** | I. MCQ | Go-Back-N behavior on frame loss | **C. Frames 2 and 3 are discarded, and retransmission begins from frame 1.** |
| **6** | I. MCQ | Reason for collisions in CSMA despite sensing | **B. Propagation delay can cause two stations to perceive the channel as idle simultaneously.** |
| **7** | I. MCQ | Principal advantage of piggybacking | **C. It combines acknowledgment information with outgoing data, reducing separate ACK frames.** |
| **8** | I. MCQ | Factor increasing jitter without distance change | **B. Variable queuing delays experienced by successive packets** |
| **9** | I. MCQ | Addressing for process-to-process delivery | **C. Port address** |
| **10** | I. MCQ | Meaning of HDLC RNR supervisory frame | **B. The receiver is currently not ready to receive further frames.** |
| **11** | II. Fill in Blank | Variation in packet arrival delay | **Jitter** (or **Delay Jitter**) |
| **12** | II. Fill in Blank | Complete message stored before forwarding | **Message** switching (Store-and-forward) |
| **13** | II. Fill in Blank | HDLC field used for CRC-based error detection | **FCS** (Frame Check Sequence) |
| **14** | II. Fill in Blank | Max normalized throughput: Slotted vs Pure ALOHA | **0.368** (or **36.8%** / $\frac{1}{e}$) compared with **0.184** (or **18.4%** / $\frac{1}{2e}$) |
| **15** | II. Fill in Blank | Retransmission-based error-control mechanism | **ARQ** (Automatic Repeat reQuest) |
| **16** | III. True/False | Transport layer flow control scope | **False** |
| **17** | III. True/False | Circuit switching routing decisions per data unit | **False** |
| **18** | III. True/False | HDLC ABM equal responsibility | **True** |
| **19** | III. True/False | Carrier sensing vs collision detection distinction | **True** |
| **20** | III. True/False | Go-Back-N physical arrival vs acceptance | **True** |

---

## Detailed Solutions & Explanations

### I. Multiple-Choice Questions (10 × 1 = 10 Marks)

#### 1. In the OSI model, which combination correctly distinguishes the responsibilities of the Data Link and Network layers?
- **A.** Data Link: process-to-process; Network: hop-to-hop
- **B.** Data Link: hop-to-hop; Network: source-to-destination
- **C.** Data Link: source-to-destination; Network: process-to-process
- **D.** Both provide only end-to-end delivery

> **Answer:** **B. Data Link: hop-to-hop; Network: source-to-destination**  
> **Explanation:**
> - The **Data Link Layer** (Layer 2) is responsible for node-to-node or **hop-to-hop** delivery of frames between two directly connected devices across a single physical link.
> - The **Network Layer** (Layer 3) is responsible for host-to-host or **source-to-destination** packet delivery across multiple intermediate routers and networks.
> - *(Process-to-process delivery is the responsibility of the Transport Layer, Layer 4).*

---

#### 2. Consider two packets belonging to the same message. They reach the destination through different routes and arrive out of order. Which switching mechanism most naturally permits this behavior?
- **A.** Circuit switching
- **B.** Message switching
- **C.** Datagram packet switching
- **D.** Virtual-circuit packet switching

> **Answer:** **C. Datagram packet switching**  
> **Explanation:**
> - In **datagram packet switching** (a connectionless switching technique), each packet is treated as an independent entity (datagram). Routers evaluate and route each packet dynamically based on network conditions at that moment. As a result, packets of the same message can take different paths, experience different delays, and arrive out of order at the destination.
> - In **virtual-circuit packet switching**, a predetermined logical connection is established prior to data transfer, and all packets follow that same path in order.
> - In **circuit switching**, a dedicated physical channel is reserved throughout the session.
> - In **message switching**, the entire message travels as a whole single unit.

---

#### 3. Consider two networks:
- **Network X:** Average delay = 25 ms; successive packet delays vary from 24–26 ms.
- **Network Y:** Average delay = 20 ms; successive packet delays vary from 5–50 ms.

**For a delay-sensitive real-time application, which conclusion is most defensible based only on the performance concepts in the module?**
- **A.** Y must always perform better because its average delay is lower
- **B.** X may provide smoother delivery because its delay variation is substantially smaller
- **C.** Both must perform identically because throughput is unspecified
- **D.** X necessarily has greater bandwidth

> **Answer:** **B. X may provide smoother delivery because its delay variation is substantially smaller**  
> **Explanation:**
> - Real-time streaming applications (such as VoIP, audio/video conferencing, and live gaming) are highly sensitive to **jitter** (the variation in packet arrival delay).
> - In Network X, delay variation is very small ($26 - 24 = 2\text{ ms}$), meaning packet inter-arrival times are highly consistent, resulting in smooth playback without excessive buffering.
> - In Network Y, delay variation is severe ($50 - 5 = 45\text{ ms}$), which causes high jitter, leading to choppy media playback, buffer underruns, or discarded late packets despite having a slightly lower average delay.

---

#### 4. Which statement correctly distinguishes throughput from bandwidth?
- **A.** Throughput is always greater than bandwidth.
- **B.** Bandwidth represents achieved data rate, whereas throughput represents theoretical capacity.
- **C.** Bandwidth represents theoretical capacity, whereas throughput represents the actual achieved rate.
- **D.** They are necessarily identical for a congestion-free network.

> **Answer:** **C. Bandwidth represents theoretical capacity, whereas throughput represents the actual achieved rate.**  
> **Explanation:**
> - **Bandwidth** measures the maximum theoretical data transfer rate or capacity of a link (e.g., a link capacity rated at 100 Mbps).
> - **Throughput** is the actual amount of data successfully transferred over the link per unit of time in practice (e.g., achieving 80 Mbps due to network overhead, processing, latency, and congestion). In practice, $\text{Throughput} \le \text{Bandwidth}$.

---

#### 5. In Go-Back-N, frames 0, 1, 2 and 3 are transmitted. Frame 1 is lost, while frames 2 and 3 reach the receiver. According to the behavior described in the module, what happens?
- **A.** Frames 2 and 3 are buffered indefinitely.
- **B.** Only frame 1 is retransmitted.
- **C.** Frames 2 and 3 are discarded, and retransmission begins from frame 1.
- **D.** Frame 1 is ignored because later frames arrived.

> **Answer:** **C. Frames 2 and 3 are discarded, and retransmission begins from frame 1.**  
> **Explanation:**
> - In **Go-Back-N ARQ**, the receiver has a window size of 1 ($R_w = 1$) and can only accept frames strictly in sequence.
> - Because frame 1 was lost, frames 2 and 3 arrive out of sequence. The receiver rejects and **discards** frames 2 and 3 without buffering them.
> - When the sender's timer expires (or upon negative acknowledgment), the sender must "go back N" and **retransmit starting from frame 1** (i.e., frames 1, 2, and 3).

---

#### 6. Why can collisions still occur in CSMA even though a station senses the channel before transmission?
- **A.** Frames do not contain addresses.
- **B.** Propagation delay can cause two stations to perceive the channel as idle simultaneously.
- **C.** CSMA does not permit retransmission.
- **D.** Carrier sensing works only after a collision.

> **Answer:** **B. Propagation delay can cause two stations to perceive the channel as idle simultaneously.**  
> **Explanation:**
> - In CSMA (Carrier Sense Multiple Access), a station listens to the medium before transmitting. However, signals propagate through the medium at a finite speed (propagation delay).
> - If station A transmits a frame, there is a time window (the vulnerable period equal to propagation delay) before station A's signal reaches station B. During this interval, station B senses the line, perceives it as idle, and begins transmission. Both transmissions then collide along the medium.

---

#### 7. What is the principal advantage of piggybacking in full-duplex communication?
- **A.** It eliminates error detection.
- **B.** It increases the frame sequence-number space.
- **C.** It combines acknowledgment information with outgoing data, reducing separate ACK frames.
- **D.** It eliminates the need for flow control.

> **Answer:** **C. It combines acknowledgment information with outgoing data, reducing separate ACK frames.**  
> **Explanation:**
> - **Piggybacking** is a technique used in two-way (full-duplex) communication where acknowledgment information for received frames is embedded (piggybacked) into the header of an outgoing data frame instead of sending a dedicated, separate ACK frame.
> - This significantly saves bandwidth and decreases transmission overhead on the channel.

---

#### 8. Which situation can increase jitter without necessarily changing the physical propagation distance?
- **A.** Replacing copper with a shorter cable
- **B.** Variable queuing delays experienced by successive packets
- **C.** Increasing signal propagation speed
- **D.** Reducing packet header size to zero

> **Answer:** **B. Variable queuing delays experienced by successive packets**  
> **Explanation:**
> - Total delay is the sum of processing delay, queuing delay, transmission delay, and propagation delay.
> - With a fixed physical route, propagation distance and speed remain constant. However, as intermediate routers experience dynamic shifts in traffic and queue occupancy, successive packets spend varying amounts of time waiting in buffers (**queuing delay**). This fluctuation is the primary cause of network **jitter**.

---

#### 9. A TCP/IP host successfully determines the destination IP address from a domain name but still cannot deliver the application data to the correct process on the destination host. Which addressing information is most directly relevant to the unresolved problem?
- **A.** MAC address
- **B.** IP address
- **C.** Port address
- **D.** DNS authoritative-server address

> **Answer:** **C. Port address**  
> **Explanation:**
> - The IP address directs the packet to the destination host machine (host-level addressing).
> - To multiplex/demultiplex and deliver data to the correct specific application process running inside that operating system, the Transport Layer relies on **Port addresses** (port numbers).

---

#### 10. An HDLC receiver sends RNR. What should the sender infer?
- **A.** The receiver has detected a CRC error.
- **B.** The receiver is currently not ready to receive further frames.
- **C.** The receiver requests selective retransmission.
- **D.** The receiver requests retransmission using Go-Back-N.

> **Answer:** **B. The receiver is currently not ready to receive further frames.**  
> **Explanation:**
> - In High-Level Data Link Control (HDLC), **RNR** stands for **Receive Not Ready** (a Supervisory frame / S-frame).
> - It acknowledges all frames up to $N(R) - 1$, but informs the sender that due to temporary conditions (such as buffer congestion or processing load), the receiver is currently not ready to accept further I-frames (flow control).

---

### II. Fill in the Blank Questions (5 × 1 = 5 Marks)

#### 11. The variation in packet arrival delay over time is called __________.
> **Answer:** **Jitter** (or **Delay Jitter**)  
> **Explanation:** Jitter represents the statistical dispersion or variance in the delay of successive packets reaching their destination.

#### 12. A switching technique in which the complete message is stored at an intermediate node before being forwarded is called __________ switching.
> **Answer:** **Message**  
> **Explanation:** In **message switching**, intermediate switches receive the entire message, store it completely in local buffers, verify it, and then forward it to the next node (known as the *store-and-forward* technique).

#### 13. The HDLC field used for CRC-based error detection is called the __________.
> **Answer:** **FCS** (or **Frame Check Sequence**)  
> **Explanation:** The HDLC frame format includes a 16-bit or 32-bit **Frame Check Sequence (FCS)** field at the end of the frame, which contains the Cyclic Redundancy Check (CRC) bits calculated over the frame contents.

#### 14. The maximum normalized throughput of Slotted ALOHA is approximately ________, compared with ________ for Pure ALOHA.
> **Answer:** **0.368** (or **36.8%** / $\frac{1}{e}$), compared with **0.184** (or **18.4%** / $\frac{1}{2e}$)  
> **Explanation:**
> - In **Pure ALOHA**, maximum channel throughput occurs at traffic load $G = 0.5$, yielding $S_{\max} = G e^{-2G} = \frac{1}{2e} \approx 0.184$ (18.4%).
> - In **Slotted ALOHA**, synchronizing transmissions into discrete time slots halves the vulnerable period, yielding maximum throughput at $G = 1.0$: $S_{\max} = G e^{-G} = \frac{1}{e} \approx 0.368$ (36.8%), roughly double that of Pure ALOHA.

#### 15. The retransmission-based error-control mechanism used in noisy-channel protocols is known as __________.
> **Answer:** **ARQ** (or **Automatic Repeat reQuest**)  
> **Explanation:** **Automatic Repeat reQuest (ARQ)** is the standard error-control strategy where corrupted or lost frames are automatically retransmitted based on acknowledgments (ACKs) and timeouts (e.g., Stop-and-Wait ARQ, Go-Back-N ARQ, Selective Repeat ARQ).

---

### III. True/False Questions (5 × 1 = 5 Marks)

#### 16. The transport layer performs flow control only between two directly connected nodes.
> **Answer:** **False**  
> **Explanation:** Flow control between two directly connected (adjacent) nodes is the responsibility of the **Data Link Layer**. The **Transport Layer** provides **end-to-end** flow control between the ultimate source host and destination host (process-to-process), spanning across intermediate routers and networks.

---

#### 17. In circuit switching, routing decisions are normally required for every unit of data after the dedicated circuit has been established.
> **Answer:** **False**  
> **Explanation:** In circuit switching, routing and path selection are performed **only once** during the initial connection setup phase. Once the dedicated physical circuit is established, all data units travel along this pre-allocated path with zero per-packet routing decisions until the session is terminated.

---

#### 18. HDLC asynchronous balanced mode gives both stations equal responsibility for controlling the link.
> **Answer:** **True**  
> **Explanation:** In HDLC, **Asynchronous Balanced Mode (ABM)** is used in balanced point-to-point configurations where both stations act as combined stations. Both stations possess equal status, equal authority, and equal responsibility to initiate commands and send responses without requiring permission from one another.

---

#### 19. Carrier sensing and collision detection are different functions; therefore, a station can sense an idle medium and still subsequently experience a collision.
> **Answer:** **True**  
> **Explanation:** **Carrier Sensing** is checking the medium *before* transmitting, whereas **Collision Detection** is monitoring the medium *during* transmission. Due to non-zero propagation delay, a station can sense an idle channel and begin transmitting, only to collide with a transmission initiated by another station moments earlier whose signal had not yet arrived.

---

#### 20. In Go-Back-N, successful physical arrival of a frame at the receiver does not necessarily imply that the frame will be accepted.
> **Answer:** **True**  
> **Explanation:** In Go-Back-N ARQ, the receiver window size is 1 ($R_w = 1$), meaning the receiver strictly accepts only frames in consecutive sequential order. If an undamaged frame arrives physically at the receiver out of order (such as when a preceding frame was dropped) or as a duplicate, the receiver automatically discards it.
