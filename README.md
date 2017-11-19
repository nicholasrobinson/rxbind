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

| PIN | Colour | Function |
| --- | ------ | -------- |
| 1 | Orange | 3.3VDC +/-5%, 20mA max |
| 2 | Black | GND |
| 3 | Gray | DATA |

To achieve this on the Pi, where we don't know when the unit powers on
and we only have a short period of time to start the binding sequence
(200ms), we use GPIO pin 3 as an input connected to the power pin. So
the steps are to wait for PIN three to go HIGH which indicates the
unit is powered up. Then delay for around 70ms, long enough for the
thing initialize but within the 200ms window. We then issue 9 pulses which
is the required number for DSMX mode (11ms internal).

# Usage
`make run` should do everything you need.
