# rxbind

Raspberry Pi code to put a Spektrum DSMX Remote Receiver SPM9645 into
bind mode.

This is achieved by a series of pulses on power-up, as stated in the
[Spectrum Guide](https://www.spektrumrc.com/ProdInfo/Files/Remote%20Receiver%20Interfacing%20Rev%20A.pdf)

> To put a receiver into bind mode, within 200ms of power application
> the host device needs to issue a series of falling pulses. The number
> of pulses issued selects which bind types will be accepted from the
> transmitter. Selecting the 11ms mode automatically allows either 11ms
> or 22ms protocols to be used. Selecting DSMx will automatically allow
> DSM2 to be used if the transmitter is unable to support DSMX. For
> this reason, we recommend that 11ms DSMx be used (9 internal or 10
> external pulses).

The interface to the remote receivers is a 3-wire bus. Pins are as
noted below:-

| Pin | Colour | Function | GPIO Pin
| --- | ------ | -------- | -------- |
| 1 | Orange | 3.3VDC +/-5%, 20mA max | 3 |
| 2 | Black | GND | GND |
| 3 | Gray | DATA | 2 |

To achieve this on the Pi, where we don't know when the unit powers on
and we only have a short period of time to start the binding sequence
(200ms), we use GPIO pin 3 as an input connected to the power pin. So
the steps are to wait for PIN three to go HIGH which indicates the
unit is powered up. Then delay for around 70ms, long enough for the
thing initialize but within the 200ms window. We then issue 9 pulses which
is the required number for DSMX mode (11ms internal).

# Usage

Clone and build the repo using the following command:
```
$ git clone https://github.com/kalopa/rxbind.git
$ cd rxbind
$ make
```

This should pull down the code and compile it. If there are errors,
these should be addressed before proceeding.

Ideally, use a breadboard to wire the SPM9645 to the Pi.

Wire the SPM9645 black wire to GND on the Raspberry Pi.

Wire the gray (DATA) wire to GPIO Pin 2.

In order to detect when the unit powers up, the power pin (the
orange wire) needs to be wired to the 3V3 output on the Pi and also
onto GPIO pin 3. To begin with, just wire it to the GPIO pin with
another jumper wire to be connected to 3V3.

Don't connect it to 3V3 just yet.

`make run` should do everything you need.

You should see the following:

```
$ make run
Waiting for device power...
```

The code should stop at this point. If it doesn't, check the 3V3 sense (GPIO pin 3).

If it has stopped waiting for device power, wire GPIO pin 3 and the orange wire to 3V3 on the Pi.

The code should complete and the output should look like this:

```
$ make run
Waiting for device power...
Begin!
Receiver in BIND mode.
```
