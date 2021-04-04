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
<mode: DC | GEN | TWOPORT>
```

The header must continue with a word specifying the analysis mode.

possible values:
- DC - solves for all voltages and currents
- GEN - solves for equivalent generator (Thevenin and Norton) parameters
- TWOPORT - solves for all possible two-port characteristic matrices

If omitted, it defaults to DC

### Analysis parameters

Additional analysis parameters might follow after the mode

#### DC

DC has no parameters, anything after mode will be ignored

#### GEN

```
<DEVICEID: unsigned> [<R1: double> <R2: double>]
```

GEN has the following parameters:

- DEVICEID: unsigned integer id of a device that will be used as "output". It's equation will be replaced, so it can be anything but a resistor is recommended
- R1, R2: resistance values to terminate the network with. Default is 1 and 1M

#### TWOPORT

```
<PRIMARYID: unsined> <SECONDARYID: unsigned>
```

- PRIMARYID: id of the primary port of the two-port network
- SECONDARYID: id of the secndary port of the netwrok

Both devices will be removed, so setting them as a resistor is recommended

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

### Short / wire

- type string: `WIRE`
- no parameters
- equation: `u = 0`
- useful for current-controlled devices

### Break / probe

- type string: `PROBE`
- no parameters
- equation: `i = 0`
- useful for voltage-controlled devices

### Coupled devices

These devices should come in pairs, but it's not enforced, so it's the responsibility of the creator of the file to ensure that.

#### Current controlled current source

- type string: `CCCS`
- parameters: source (id), current gain (double a)
- equation: `a * i(id) - i = 0`

#### Current controlled voltage source

- type string: `CCVS`
- parameters: source(id), transfer resistance (double r)
- equation: `r * i(id) - u = 0`

Since there's no gyrator component, two of these can be used instead easily

#### Voltage controlled voltage source

- type string: `VCVS`
- parameters: source(id), voltage gain (double g)
- equation: `g * u(id) - u = 0`

#### Voltage controlled current source

- type string: `VCCS`
- parameters: source(id), transfer conductance (double g)
- equation: `g * u(id) - i = 0`

#### Nullator

- type string: `NULL`
- parameters: friend device (id)
- equations: `u=0, i=0`
- since every device can have only one equation, it overrides the equation of it's 'friend' device
- this means the other device effectively becomes a norator, and the two devices together form a nullor, or an ideal amplifier
- the writer of a file must be careful with the friend devices!
- using `AMP` shorthand is recommended instead

### Coupled pair shorthand

When a line starts with a dollar sign, it's a shorthand for a coupled device pair

```
$ <type: string> <id> <plus1: node> <minus1: node> <plus2: node> <minus2: node> <param: double>
```

Don't forget that they still count as 2 devices, taking `id` and `id+1`!

#### ideal transformer

- type string: `TRAN`
- parameter is ratio between first and second device
- synthetised with a CCCS and VCVS

#### gyrator

- type string: `GYR`
- parameter is gyration resistance
- syntethised with two CCVS-es

#### Ideal amplifier

- type string: `AMP`
- no parameters
- syntethised with a nullator and a short (replaced with a norator)


## Empty lines, comments or EOF

- empty lines are ignored
- also every line starting with hashtag (#) is ignored
- (due to the way parsing is implemented any extra info written after a line is also ignored IF NO PARAMETERS CAN FOLLOW)
- parsing terminates without error when EOF is reached or a line contains `END`
