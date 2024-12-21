# vfdrtu

Client and Server programs for testing Modbus RTU communication with a VFD.

## Getting Started

To build, run the build script located in the root directory of the project.

```bash
./build.sh
```

This will create a `build` directory; the executables will be located in the
`build/bin` directory.

## Usage

### Server

The server program is used as a substitute for the VFD to observe the Modbus
RTU commands it is receiving from clients. Here is an example of a command to
run the server with communication on `/dev/ttyUSB0`:

```bash
./build/bin/server -p /dev/ttyUSB0
```

### Client

The client program is used to send commands to the VFD. Here is an example of
a command to run the client with communication on `/dev/ttyUSB0`:

```bash
./build/bin/client -p /dev/ttyUSB0
```
