# Wavious LPDDR SW
Wavious LPDDR Software project. This project contains necessary device driver
and logic for controlling Wavious LPDDR Hardware.
See [wav-lpddr-hw](https://github.com/waviousllc/wav-lpddr-hw) for more details
about the hardware platform.

## Project Organization
At a high-level the project is organized into 5 main directories:

### Real Time Operating System (rtos)
RTOS contains all of the source code for FreeRTOS and any additons that have
been added by Wavious for running FreeRTOS on Wavious developed RISC-V hardware.

### Drivers (drivers)
Drivers are the layer that interacts directly with the Wavious LPDDR hardware.
These functions are identified by the `reg_if` suffix. All interactions with the
hardware consolidated to this layer in order to make the code easier to
maintain and debug. The driver layer sits on top of the RTOS kernel and relies
on `reg_read` and `reg_write` functions for manipulating CSRs.

### Devices (dev)
Devices are higher abstractions of device drivers. Devices can wrap underlying
drivers in order to provide better abstractions or even combine multiple drivers
and device to create a higher-level device. Devices sit at the layer above
drivers.

### Finite State Machines (fsm)
Finite state machines are the highest level of device abstraction. They provide
a standard interface for manipulating one or more devices that have a complex
interaction model. A great example of this is the Frequency Switch FSM which
relies on PLL and Frequency Switch hardware being in sync. FSMs are built on top
of the standard FSM module that is part of the RTOS kernel.

### Applications (app)
Applications are complete programs that are meant to run on LPDDR hardware.
These applications can be "fully baked" mission type applications (`wddr_boot`)
or simple apps that test only a specific device or driver. Typically, Wavious
builds several test applications used to tests SW interaction with various
software components. For this repository, only mission-mode applications
have been included.

## Project Initialization
  - `git clone https://github.com/waviousllc/wav-lpddr-sw.git`
  - `cd wav-lpddr-sw`
  - `git submodule update --init --recursive`

## Building
It is recommended that Docker is used for the build environment. Build the Docker
Image using the `Dockerfile` located under the `docker` directory of this
project. Once built, use `./run.sh` script to create a shell within the Docker
environment. All code is linked into the container via a Docker volume. Thus,
changes persist between runs and edits are shared outside and within the
Docker container.

### Building Docker
`docker build -t <IMAGE_NAME>:latest docker/`

### Building Software
~~~
./docker/run.sh <IMAGE_NAME>
---- In Docker Container ----
./configure (only required once)
cd build
make
~~~

### Configurations
| Varaible Name            |  Default Value |    Description                        |
|--------------------------|----------------|---------------------------------------|
| CONFIG_CALIBRATE_PLL     |    true        | Enables PLL calibration at boot       |
| CONFIG_CALIBRATE_ZQCAL   |    true        | Enables ZQCAL calibration at boot     |
| CONFIG_CALIBRATE_SA      |    true        | Enables Sense Amp calibration at boot |

#### Changing Configurations
It is recommended that all binaries are built with the default configuration. However,
in some instances, it might be convenient to disable certain features while debuging
issues.

After running the commands given in the [Building](#building) section of this document,
the configuration can be updated as follows:
~~~~
cd build
cmake .. -DCONFIG_CALIBRATE_PLL=<true|false> -DCONFIG_CALIBRATE_ZQCAL=<true|false> -DCONFIG_CALIBRATE_SA=<true|false>
make
~~~~

The generic command: `cmake .. -D<CMAKE_VARIABLE_NAME>=<VAL>`

## Testing
See [wav-lpddr](https://github.com/waviousllc/wav-lpddr) for testing SW builds.
