# Wavious LPDDR SW
Wavious LPDDR Software project. This project contains necessary device driver
and logic for controlling Wavious LPDDR Hardware.
See [wav-lpddr-hw](https://github.com/waviousllc/wav-lpddr-hw) for more details
about the hardware platform.

## License
This project is licensed as Apache-2.0. Previous releases used GPL-3.0 and
LGPL-3.0.

## Project Organization
At a high-level the project is organized into 6 main directories:

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

### Target (target)
Targets are specific implementations of the LPDDR PHY. Different targets have
different features or even memory maps. All devices and drivers exists within
the directories described above, regardless of which targets supports those
features. Each target selects the source code and header files for these
entities and builds the particular set of drivers and devices that it needs for
its implementation.

The build system is setup such that a particular target can be built. To build
for multiple targets, one must rerun the `configure` script, specifying the
new target.

### PHY Firmware (firmware)
PHY Firmware is the management entity that controls the PHY, as well as performs
state management. It responds to events from DFI interface (via IRQs) or events
requested through the PHY Firmware API. The PHY Firmware API encapsulates all PHY
features that an application can control, such as frequency switch prep. It
interacts and controls the PHY through the PHY interface API.

### Applications (app)
Applications are complete programs that are meant to run on LPDDR hardware.
Applications should interact with the PHY via the PHY Firmware API. They are
not able to control or update the PHY directly. Typically, applications can be
"fully baked" mission type applications (`wddr_boot`) or test applications.
For this repository, only mission-mode applications have been included.

## Project Initialization
  - `git clone https://github.com/waviousllc/wav-lpddr-sw.git`
  - `cd wav-lpddr-sw`
  - `git submodule update --init --recursive`

## Building
It is recommended that Docker is used for the build environment. Build the Docker
Image using the `Dockerfile` located under the `docker` directory of rtos submodule.
Once built, use `./docker/run.sh` script to create a shell within the Docker
environment. All code is linked into the container via a Docker volume. Thus,
changes persist between runs and edits are shared outside and within the
Docker container.

### Building Docker
`docker build -t <IMAGE_NAME>:latest rtos/docker/`

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
| CONFIG_DRAM_TRAIN        |    false       | Enables DRAM Training at boot         |
| DCONFIG_CAL_PERIODIC     |    false       | Enables PHY Periodic Calibration      |
| DCONFIG_TARGET_PHY       |    wddr100     | Selects PHY Target to build           |

#### Changing Configurations
It is recommended that all binaries are built with the default configuration. However,
in some instances, it might be convenient to disable certain features while debuging
issues.

After running the commands given in the [Building](#building) section of this document,
the configuration can be updated as follows:
~~~~
cd build
cmake .. -DCONFIG_CALIBRATE_PLL=<true|false> -DCONFIG_CALIBRATE_ZQCAL=<true|false> -DCONFIG_CALIBRATE_SA=<true|false> -DCONFIG_DRAM_TRAIN=<true|false> -DCONFIG_CAL_PERIODIC=<true|false> -DCONFIG_TARGET_PHY=wddr100
make
~~~~

The generic command: `cmake .. -D<CMAKE_VARIABLE_NAME>=<VAL>`

### Adding Extended Functionality to Builds
In order to allow for extended capabilites not required for PHY functionality
(such as training), each target creates an extension library named wddr_ext.
In order to utilize this library, one can add additional source and
include files to this library to be included when building a particular target.
This will enable external functions to be added to the build to
override weak functions within the WDDR device (such as wddr_train).

## DRAM Training
Currently, the WDDR device includes the necessary hooks to perform DRAM training
during boot of the PHY, but the algorithms are not included in this release.
These algorithms can be added using the wddr_ext interface library. If training
is a requirement of your application, contact Wavious for support.
