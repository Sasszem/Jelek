# Automating the analysis of linear resistive networks

This project is an automated analyzer for linear resistive networks. These networks are modelling a subset of real-life circuits, and thus can be useful for understanding them.

**DISCLAIMER: the terminology in this document is a near direct translation of the Hungarian terminology, and thus, not guaranteed to match the currently used English terminology, but all the general concepts are the same.**

See also
- Doxygen documentation
- [file format documentation](file_format.md)

## Linear math

The project relies heavily on vector and matrix operations, and for this purpose, implements it's own matrix class with all needed functionality. It supports addition, scalar multiplication, matrix-matrix multiplication, sub-matrix extraction, transposition and inversion.

There is no dedicated vector class, vectors can be represented as a 1xN or Nx1 matrix instead.

A very important structure used is a linear equation system represented as it's extended matrix.

A basic inhomogenous linear equation system represented by matrix M and vector V for unknown vector X is written as follows:

M * X = V

V will be referred as the vector of the equation system or as the right-hand side (RHS), and the matrix M will just be referred as the matrix.

Instead of storing two matrices, it is easier to store only one, called the extended matrix.

This N x (N+1) matrix has M in the first N columns, and the last column is the RHS.

## Devices

Linear resistive networks are built from different **one-port** (2-terminal) and **two-port** (4 terminal) **devices**. Each device has a **voltage drop** and a **current trough it**. Their direction matters, so we also have **reference directions** for every device - *in this project reference directions for current and voltage are the same for the same device*.    
Devices can have characteristic equations describing the relationship between their voltage and current. For linear resistive networks, those equations are always linear, and time-indipendent.   

### non-coupled two terminal devices
The equations of non-coupled devices does not contain any unknowns other than the voltage and current of the said device.

- Voltage source - `u = U0`
- Current source - `i = I0`
- Resistor - `i*R - u = 0`
- Probe ("break", "0-current source") - `i = 0`
- Wire ("short", "0-voltage source") - `u = 0`

### coupled two terminal devices
Those devices can also have the voltages or currents of any other device in their characteristic equations

- voltage controlled voltage source (VCVS) - `u(self) - μ*u(other) = 0` - the parameter μ (denoted Greek letter mu) is often called *voltage gain*
- voltage controlled current source (VCCS) - `i(self) - g*u(other) = 0` - the parameter g is often called *transfer conductance*
- current controlled current source (CCCS) - `i(self) - α*i(other) = 0` - the parameter α (denoted Greek letter alpha) is often called *current gain*
- current controlled voltage source (CCVS) - `u(self) - R*i(other) = 0` - the parameter R is often called *transfer resistance*

### coupled 4 terminal devices
There can be many variations, but the ones described here are built from two coupled two terminal devices (thus current flowing into each port is the same as flowing out from the same port). They all have two equations.

- ideal transformer with 1:n ratio - `u(second) - n*u(first) = 0` and `i(first) - n*i(second) = 0`
- ideal amplifier (also known as nullor) - `u(first) = 0` and `i(first) = 0`
- gyrator - `u(second) - r*i(fist) = 0` and `u(first) + r*i(first) = 0`. The parameter r is often called gyration resistance

## Implementation details of devices in this project

This project does not implement all the devices described above, namely it does not implement ideal transformers and gyrators. Those devices are instead can be built from controlled sources (transformer is a CCCS and a VCVS cross-coupled and gyrator is two cross-coupled CCVS). For helping with this, I implemented a short-hand syntax that creates those device pairs at once.

All devices are implemented as inherited classes from the abstract base class `IDevice`. This common interface describes terminal connections to nodes, have an ID, debug and helper functions, and a function allowing the device to write it's equation(s) into a matrix.

Since we sometimes have to store a device reference AND a direction compared to a reference direction, I used positive integers as IDs, and opposite-direction is represented as a negative value. This only happens while generating network equations.

The number of equations allocated is exactly the number of devices in the network, and equations are in the same order as device IDs, and also written to the matrix of the equation system in the same order. 

### problems with ideal amplifier

This poses a problem with the ideal amplifier, as it's primary device (named *Nullator* in this project) has two equations, and the secondary has none. This is solved by the primary device writing to the equation allocated for the secondary device. There is no special no-equation device implemented for that, instead, any device can act as a secondary side, it's equation will be overwritten. This also means the equation of the secondary device must be written before the equations of the primary, i.e. secondary device should have lower ID than the primary. Also extra care must be taken so no secondary device is reused.

All those constraints are ensured when using a short-hand form for creating both devices at the same line.

### Equation function

This function takes a matrix by reference and returns a double. It should write the coefficients of unknowns to the matrix, and return the right-hand side constant. Most devices only write one equation, except the Nullator, as described above.

## Network

A network is a collection of devices with extra information describing their connections. Points at witch devices connect together are called *nodes*, and devices themselves are often called branches. A network can be described as a directed graph.

### Equations

There are two types of constraints the connections create:
- node laws (Kirchhoff's current laws) - the signed sum of currents at a node must be 0
- cycle laws (Kirchhoff's voltage laws) - the signed sum of all voltages in a cycle must be 0

In every component of the network, if that component has N nodes and B branches, we can write `N - 1` independent current laws and `B - N + 1` independent voltage laws. Those and the `B` equations of the devices form the complete system of network equations. This equation system is generated for the network, and used for analysis.

### Constructing equations

#### Node laws

Node laws are the simpler of the two kinds. All we need is to have the network in directed outgoing adjacency list format, and for each node, write 1s and -1s as coefficients. Since their total sum is 0, we don't even have to care witch direction is witch.

Selecting N-1 nodes per component is not hard either. For voltage laws, I've already implemented a DFS traversal, witch can also be used to select one root node per component. Writing node equations for every non-root node results in a correct system.

#### Voltage laws

Generating cycles for a graph is not the most straightforward algorithm, but not very hard either. 

A simple solution is to generate a spanning forest of the graph, "flag" binding / non-tree branches and find cycles induced by them. 

For each component, the spanning tree will have `N - 1` branches, and thus, we find `B - N + 1` binding branches. Furthermore, each binding branch defines a unique cycle.

This can simply be done by tracing a route between their two terminals consisting only of tree branches, and closing the cycle trough the binding branch.

I went for a DFS algorithm for finding the binding branches, witch also recorded for each node the signed ID of the branch that node was discovered from. This allows the easy construction of paths between any two node by backtracing to root and removing the same elements from the end (the path from their closest common ancestor to root).

## Analysis

There can be many different types of analysis, and I've only implemented the most common ones.

Some networks can not be solved. Those are called irregular networks, and result in a message stating this fact.

The different solvers are implemented as different classes inherited from abstract base class `INetworkSolver`. They all have a `solve()` method taking the extended matrix of the equation system and they return a result matrix (no fixed size), witches interpretation depends on the specific solver.

### Equation system printing

Implemented in class `EquationSystemSolver`

This solver takes a single optional parameter `printType`, with two possible values (1 or 2). 1 means human readable, 2 means print as matlab code.


This problem asks for the equation system describing the network. This can be useful in development, but also if the program can not do the analysis we need so we work on this equation system on our own.

This is done simply by printing the matrix and vector of the equation system.

This solver returns a 0x0 matrix.

### DC analysis

Implemented in class `NetworkSolverDC`

This solver takes no parameters.

This problem asks for some or all voltages or currents in a network.

Solving for those unknowns is trivial, as it means simply solving the equation system. This can be easily done by inverting the matrix and multiplying by the RHS.

The result vector thus will be the [u1; i1; u2; i2; ...] vector.

### Equivalent resistor analysis

Implemented in class `NetworkSolverResistance`

Takes a single parameter: termination device ID

If a network does not have any independent sources, it can be represented by a single (not strictly positive or finite) resistor. This analysis finds the value of this resistor.

The methodology is simple - we apply a current and "measure" voltage. An extra check is also done to ensure that we have 0 voltage for 0 current.

"measuring" simply means solving a modified equation system. We erase the equation of a device, overwrite it as a current source, and solve this equation system. For this reason, we must have a dedicated terminating device, witch can be of any kind (but not be a second port of an amplifier).

The voltage for a 1A stimuli is the resistance value we need.

### Equivalent generator analysis

Implemented in class `NetworkSolverGen`

Takes 3 parameters: termination device ID, R1 (optional, defaults to 1) and R2 (optional, defaults to 1000000)

If a network DOES have independent sources, it can be modelled as a Thévenin or Norton generator, consisting of a voltage or current source and a resistor. This analysis mode solves for the voltage, current and resistance parameters in these two generators.

Methodology is similar to the last one, but we have to record two (u, i) datapoints at two terminations R1 and R2. The two points describe a line, it's slope will be the negated resistance, and it's axis intercepts will be the voltage and current. A special case is a 0-ohm fixed current Norton generator, witch we have to check for separately.

Result will be a [U; I; R] column vector.


### Two-port analysis

Implemented in class `NetworkSolverTwoport`

Takes two parameters: primary and secondary termination device IDs

A two-port devices are a special subclass of 4-terminal devices. Two equations describe the relationships between their 4 unknowns (u1, i1, u2, i2). 

There are 6 different combinations in total, divided into 2 groups. None of them is guaranteed to exist, and their existence is somewhat independent of each other.

#### Hybrid type characteristics

The reference directions for i1 and i2 are the same as u1 and u2 (current flowing into the port)

##### Impedance characteristics
This is represented by the matrix R

`u1 = R11 * i1 + R12 * i2`
`u2 = R21 * i1 + R22 * i2`

Sidenote: if R exists, we can draw a **(hybrid)-T** equivalent network with the same characteristics with 3 resistors (and a coupled source in some cases)

##### Admittance characteristics
Represented by the matrix G

`i1 = G11 * u1 + G12 * u2`
`i2 = G21 * u1 + G22 * u2`

Sidenote: if G exists, we can draw a **(hybrid)-pi** equivalent network with the same characteristics with 3 resistors (and a coupled source in some cases)

Sidenote 2: if they both exists, R and G are each others inverse matrices.

##### Hybrid
Represented by the matrix H

`u1 = H11 * i1 + H12 * u2`
`i2 = H21 * i1 + H22 * u2`

##### Inverse hybrid
Represented by the matrix K

`i1 = K11 * u1 + K12 * i2`
`u2 = K21 * u1 + K22 * i2`

Sidenote: if they both exist, H and K are each others inverses

#### Chain-type characteristics

These two types flip the reference direction of the current of the second port.

##### Chain
Represented by the matrix A

`u1 = A11 * u2 + A21 * i2`
`i1 = A21 * u2 + A21 * i2`

##### Inverse chain
Represented by the matrix B

`u2 = B11 * u1 + B21 * i1`
`i2 = B21 * u1 + B21 * i1`

If they both exist, A and B are inverses

#### Solving for two port matrices

The methodology is very similar for all 6 of them, we only have to swap the unknowns and also the reference directions for chain type characteristics. For this analysis, we need 2 termination devices.

- remove equations of termination devices (not just wipe, discard the lines completely)
- re-order the columns of the matrix (and thus the unknowns) so we have the ones we have as parameters, and the ones we are solving for in the 3rd and 4th
- split the matrix into two, the first two columns being matrix M and the rest being matrix N. This way, N has the coefficients of all unknowns. 
- result is the negative inverse of M multiplied by N
- this solves for all unknowns, so we have to discard everything but the first 2 rows, resulting in a 2x2 matrix

This way we can solve independently for each of the 6 characteristics. For reordering rows or columns, I used permute matrices.

Result will be a 12x2 matrix built by stacing (from top to bottom) R, G, H, K, A and B. If any of them does not exist, it's place is filled with NaNs.

## Other implementation details

Namespaces (even nested) are used to separate different modules of this program.

Network class holds all devices and is responsible for generating equation systems. It also holds a pointer to a `INetworkSolver`.

Loading of the network happens from any input stream. The loader returns a single network pointer, or throws a LoadException.

Network's `solve()` method is a wrapper around the solve method of the associated solver, but all exceptions are catched and re-thrown as a SolveException with the original message.

The matrix class implements two custom exceptions, InvertException (matrix is singular) and DimensionException (anything dimension related).

For memory management, smart pointers (mostly `std::unique_ptr`s) are used. Naked pointers are only used when loading and always wrapped into a `unique_ptr` as soon as possible.

Documentation is done through doxygen.