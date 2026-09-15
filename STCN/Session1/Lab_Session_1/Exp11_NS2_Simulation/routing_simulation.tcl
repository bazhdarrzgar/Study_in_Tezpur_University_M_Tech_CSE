# ==============================================================================
# Experiment 11: Simulation of Routing Algorithms using NS-2 Simulator
# Course: Selected Topics in Computer Networks (CSMT 502)
#
# Description:
# This OTcl script simulates dynamic routing (Distance Vector - DV) in a 7-node
# network topology.
# It demonstrates dynamic routing adaptation when an active link fails and later recovers.
#
# Topology:
#       (n0) --- (n1) ------- (n4) --- (n5) [Destination]
#        |         |            |        |
#        |         |            |        |
#       (n6) --- (n2) ------- (n3) ------+
#
# Dynamic Event:
# At t = 2.0s, the primary link between n1 and n4 is brought DOWN.
# The Distance Vector protocol detects the break and dynamically reroutes
# traffic through n2 and n3 to reach n5.
# At t = 3.5s, the link (n1-n4) is brought UP, and routing reconverges.
# ==============================================================================

# 1. Create a Simulator instance
set ns [new Simulator]

# 2. Tell the simulator to use Distance Vector routing protocol
$ns rtproto DV

# 3. Define visualization colors for NAM
$ns color 1 Blue
$ns color 2 Red

# 4. Open trace files for recording
set tracefile [open out.tr w]
$ns trace-all $tracefile

set namfile [open out.nam w]
$ns namtrace-all $namfile

# 5. Define Finish procedure
proc finish {} {
    global ns tracefile namfile
    $ns flush-trace
    close $tracefile
    close $namfile
    puts "Simulation finished. Trace files 'out.tr' and 'out.nam' generated."
    exit 0
}

# 6. Create network nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]

# Label nodes for NAM display
$n0 label "Source (n0)"
$n5 label "Dest (n5)"

# 7. Create duplex links between nodes
# Syntax: $ns duplex-link <node1> <node2> <bandwidth> <delay> <queue_type>
$ns duplex-link $n0 $n1 2Mb  10ms DropTail
$ns duplex-link $n0 $n6 1Mb  20ms DropTail
$ns duplex-link $n6 $n2 1Mb  20ms DropTail
$ns duplex-link $n1 $n2 2Mb  10ms DropTail
$ns duplex-link $n1 $n4 2Mb  10ms DropTail
$ns duplex-link $n2 $n3 2Mb  10ms DropTail
$ns duplex-link $n3 $n4 2Mb  10ms DropTail
$ns duplex-link $n4 $n5 2Mb  10ms DropTail
$ns duplex-link $n3 $n5 1.5Mb 15ms DropTail

# Set link orientations for aesthetic layout in NAM
$ns duplex-link-op $n0 $n1 orient right
$ns duplex-link-op $n1 $n4 orient right
$ns duplex-link-op $n4 $n5 orient right
$ns duplex-link-op $n0 $n6 orient down
$ns duplex-link-op $n6 $n2 orient right
$ns duplex-link-op $n1 $n2 orient down
$ns duplex-link-op $n4 $n3 orient down
$ns duplex-link-op $n2 $n3 orient right
$ns duplex-link-op $n3 $n5 orient right-up

# 8. Setup UDP transport agent at source node n0
set udp [new Agent/UDP]
$udp set class_ 1
$ns attach-agent $n0 $udp

# Setup Null agent (traffic sink) at destination node n5
set null [new Agent/Null]
$ns attach-agent $n5 $null

# Connect source UDP agent to destination Null agent
$ns connect $udp $null

# 9. Setup Constant Bit Rate (CBR) traffic generator over UDP
set cbr [new Application/Traffic/CBR]
$cbr set packetSize_ 500
$cbr set interval_ 0.005
$cbr attach-agent $udp

# 10. Schedule dynamic routing events
# Traffic starts at t = 0.5s
$ns at 0.5 "$cbr start"

# Simulate Link Failure at t = 2.0s
$ns rtmodel-at 2.0 down $n1 $n4
$ns at 2.0 "puts \"\[Event t=2.0s\] Primary Link (n1-n4) DOWN! Distance Vector rerouting activated.\""

# Simulate Link Recovery at t = 3.5s
$ns rtmodel-at 3.5 up $n1 $n4
$ns at 3.5 "puts \"\[Event t=3.5s\] Primary Link (n1-n4) RESTORED! Routing reconverging to optimal path.\""

# Traffic stops at t = 4.5s
$ns at 4.5 "$cbr stop"

# Finish simulation at t = 5.0s
$ns at 5.0 "finish"

# 11. Run simulation
$ns run
