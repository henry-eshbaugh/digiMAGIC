# digiMAGIC: a digitally-compensated anisotropic magnetoresistance magnetometer

This GitHub repo hosts the firmware for my MEng thesis project at Imperial College London.
It is a magnetometer, using the HMC1001 strips from Honeywell, that is digitally-compensated,
leveraging DSP techniques in the acquisition of magnetometer samples, and using these samples
to back off the field at the sensor by driving an offset coil.

## Theoretical background

A lock-in amplifier is used to take sensitive measurements where the signal of
interest would ordinarily be swamped by in-band noise. The effect is essentially
to modulate the region of interest out of the noise regime, and to demodulate later.
This is the manner by which most sensitive AMR sensors are designed - leveraging
magnetic domain flipping and realignment to obtain this modulation.

However, most instrument front-ends are square-wave modulated, which is not ideal
for work in the Fourier domain. To be precise, the signal is *non-sparse*; we obtain
not one Dirac delta, but an infinite number of them at harmonics. The notion is -
should we project the acquired signal into another basis, we can obtain a sparse
representation of the signal ideal for sensor lock-in.

Enter wavelet theory. A *Haar basis* is a complete basis for the Hilbert space
of square-integrable functions, built on boxcar and windowed square wave signals
of finite support, known as the "father" and "mother" wavelet, or the scaling
and wavelet functions. This basis is intimately tied to a decimated, perfect-reconstruction
filter bank, which is iterated in the lowpass branch to obtain the wavelet transform.
This is Mallat's "multiresolution framework," which is implemented herein to
denoise the magnetometer error signal.

The multiresolution analysis framework, yielding us the wavelet transform,
is enormously convenient for our purposes. Not only is it straightforward
to implement (especially in the Haar case), but 


## Implementation

A 1MSPS 16-bit ADC, the AD8681, interfaces to the output of an instrument amplifier, the
AD8421. The reference pin of the instrument amplifier is driven by an inverting integrator
built with the `OP2277` operational amplifier to obtain a high-pass filter at 300Hz without
explicitly AC-coupling the sensor interface. This is a dual package; the other op-amp
is a V-to-I converter, sourcing current from the AD5689 DAC.

Power is generated from a trio of buck converters, one generating the digital rail, and
two analog rails. Further reference voltages are derived using LDOs hanging off the
analog rail buck converters. Total quiescent consumption is some 200mW.

## Project prerequisites

The user must have the ST-LINK command line utility installed, to interact with
physical hardware.

The user must install `arm-none-eabi-gcc`

## Compiling the project

The firmware can be compiled with the provided Makefile. This will generate
object files and link them into an ELF binary. This binary is suitable
for flashing the PCB, using the ST-LINK utility. The sequence is

    make
    make flash

The
    make clean

target will remove files generated during builds.

## Communicating with digiMAGIC

A USB-to-USART bridge, the FT230X, is installed on digiMAGIC.
Communication is available from the onboard micro-USB port.

The serial port operates at 115200 baud, 1 stop bit, no parity.

On Linux and Mac OS, the raw serial interface can be tapped
with `screen`. On Windows, PuTTY is recommended. The PySerial
library is convenient for writing driver scripts.

### digiMAGIC outputs

Various packets are output; they are prefixed with various ASCII
lowercase letters.

| Letter | Packet Type  | Structure                                            |
|--------|--------------|------------------------------------------------------|
| S      | Science      | %d, %d - instrument time (ms), field conversion (EU) | 
| D      | Debug        | ASCII string                                         |
| H      | Housekeeping | ASCII string                                         |    
| E      | Error        | ASCII string                                         |

Helper scripts are provided for serial communication and real-time graphing via `gnuplot`.

### digiMAGIC telecommands

Commands are supported to alter the functionality of digiMAGIC. They are summarised here.

| Command | Argument | Function |
|---------|----------|----------|
| DECIM   | integer  | 

## Project structure

The directory structure is as follows:
`include/` holds header files.
`src/` holds all C source files.
`bin/` holds outputs of the Makefile.

