# Network solver file format

## Header

```
<network parameters> <analysis mode> [analysis parameters]
```

### Network parameters

```
<N: unsigned> <B: unsigned>
```

The header must consist of 2 unsigned integers N and B
- N is the number of nodes in the network
- B is the number of devices (branches)

### Analysis mode

```
<mode: DC | GEN | TWOPORT | EQ | RES >
```

The header must continue with a word specifying the analysis mode.

possible values:
- DC - solves for all voltages and currents
- GEN - solves for equivalent generator (Thevenin and Norton) parameters
- TWOPORT - solves for all possible two-port characteristic matrices
- EQ - diagnostic printing of the extended matrix of the equation system
- RES - equivalent resistor parameters

If omitted, it defaults to DC

### Analysis modes and parameters

Additional analysis parameters might follow after the mode

#### DC

DC has no parameters, anything after mode will be ignored

Return will be a column vector with every device's voltage and current, sorted by id, so `u1, i1, u2, i2, ...`

#### GEN

```
<DEVICEID: unsigned> [<R1: double> <R2: double>]
```

GEN has the following parameters:

- DEVICEID: unsigned integer id of a device that will be used as "output". It's equation will be replaced, so it can be anything but a resistor is recommended
- R1, R2: resistance values to terminate the network with. Default is 1 and 1M

Result will be a 3x1 column vector `[u, i, r]`

#### TWOPORT

```
<PRIMARYID: unsined> <SECONDARYID: unsigned>
```

- PRIMARYID: id of the primary port of the two-port network
- SECONDARYID: id of the secondary port of the network

Both devices will be removed, so setting them as a resistor or wire is recommended

Result will be a 12x2 matrix, where 2x2 blocks are the R, G, H, K, A and B matrices in this order from top to bottom.

**WARNING:** I got some funny results when NOT using device 1 and 2 as the ports, witch I'll look furter into later.


#### EQ

```
[printType: 1 | 2]
```

Parameters
- `printType`. 1 for human-readable, 2 for matlab-code. Defaults to 1 (human-readable)

Result is the extended matrix of the equation system or matlab code declarinig matrix and vector of the equation system

#### RES

```
port: device
```

Calculates equivalent resistor value. Does a basic sanity check with 0-current stimulus to check if it's really a resistor.

- PORT: id of the device identifying the ports

The equation of the device specified will be erased! If it was already erased by a nullor, the answer will be wrong! 

Result is a 1x1 matrix with the resistance value.

## devices

```
<TYPE: string> <ID: unsigned> <PLUS: unsigned> <MINUS: unsigned> <...params>
```

All devices have a type, an ID, and two connections (plus and minus) to nodes.

### Resistor

- type string: `RES`
- parameter: resistance (double)
- equation: `iR - u = 0`

### Voltage source

- type string: `VOL`
- parameter: voltage (double)
- equation: `u = uset`

### Current source

- type string: `CUR`
- parameter: current (double)
- equation: `i = iset`

Caveat: it's a current SINK, so current will flow from + to -! (it follows the normal directions for voltage and current)

### Short / wire

- type string: `WIRE`
- no parameters
- equation: `u = 0`
- useful for current-controlled devices or dummy devices in TWOPORT or RESISTOR analysis

### Break / probe

- type string: `PROBE`
- no parameters
- equation: `i = 0`
- useful for voltage-controlled devices or as dummy devices

### Coupled devices

These devices should come in pairs, but it's not enforced, so it's the responsibility of the creator of the file to ensure that.

To make clear that a device is a coupled one, all coupled device types start with a dollar sign

#### Current controlled current source

- type string: `$CCCS`
- parameters: source (id), current gain (double a)
- equation: `a * i(id) - i = 0`

#### Current controlled voltage source

- type string: `$CCVS`
- parameters: source(id), transfer resistance (double r)
- equation: `r * i(id) - u = 0`

Since there's no gyrator component, two of these can be used instead easily

#### Voltage controlled voltage source

- type string: `$VCVS`
- parameters: source(id), voltage gain (double g)
- equation: `g * u(id) - u = 0`

#### Voltage controlled current source

- type string: `$VCCS`
- parameters: source(id), transfer conductance (double g)
- equation: `g * u(id) - i = 0`

#### Nullator

- type string: `$NULL`
- parameters: friend device (id)
- equations: `u=0, i=0`
- **FRIEND DEVICE ID MUST BE LOWER THEN ID!**
- since every device can have only one equation, it overrides the equation of it's 'friend' device
- this means the other device effectively becomes a norator, and the two devices together form a nullor, or an ideal amplifier
- the writer of a file must be careful with the friend devices!
- using `AMP` shorthand is recommended instead

### Coupled pair shorthand

These pseudo-devices are actually two normal devices used to build a common coupled pair. Since they are still two devices, they take the given ID and the next one.

To make clear that a device is a pair shorthand, all type strings start with an exclamation mark.

```
<type: string> <id> <plus1: node> <minus1: node> <plus2: node> <minus2: node> <param: double>
```

Don't forget that they still count as 2 devices, taking `id` and `id+1`!

#### ideal transformer

- type string: `!TRAN`
- parameter is ratio between first and second device
- synthetized with a CCCS and VCVS

#### gyrator

- type string: `!GYR`
- parameter is gyration resistance
- synthetized with two CCVS-es

#### Ideal amplifier

- type string: `!AMP`
- no parameters
- synthetized with a nullator and a short (replaced with a norator)


## Empty lines, comments or EOF

- empty lines are ignored
- also every line starting with hashtag (#) is ignored
- (due to the way parsing is implemented any extra info written after a line is also ignored IF NO PARAMETERS CAN FOLLOW)
- parsing terminates without error when EOF is reached or a line contains `END`
- don't press CTRL-C, because it throws and exception.