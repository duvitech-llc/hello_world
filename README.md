# Hello World Kernel Driver

A simple Linux kernel module demonstrating character device functionality, including read and write operations. This driver allocates a device number, creates a device file, and provides basic read/write functionality using kernel-space buffers.

## Features

- Allocates a character device with a unique major and minor number.
- Creates a device file for user-space interaction.
- Supports reading and writing data between user space and kernel space.
- Logs actions to `dmesg` for tracking module loading, unloading, and operations.

## Prerequisites

- **Linux Kernel Headers**: Ensure the headers for your target kernel are installed.
- **Cross-Compilation Toolchain** (if cross-compiling): For example, `gcc-aarch64-linux-gnu` for ARM64.
- **Kernel Version Compatibility**: This module is compatible with Linux kernel 5.15+ and uses `GPL` licensing.

## Installation

1. **Clone the Repository**:
    ```bash
    git clone <repository-url>
    cd hello_world_driver
    ```

2. **Compile the Kernel Module**:
   - **For Native Compilation**:
      ```bash
      make
      ```
   - **For Cross-Compilation** (e.g., targeting Jetson Orin ARM64):
      ```bash
      make ARCH=arm64 CROSS_COMPILE=/opt/aarch64--glibc--stable-2022.08-1/bin/aarch64-buildroot-linux-gnu-
      ```

3. **Insert the Module**:
    ```bash
    sudo insmod hello_world.ko
    ```

4. **Check Device Registration**:
   - Verify that the device file `/dev/hello_world` has been created and that the major and minor numbers are correct.
   - Use `dmesg` to see logs:
      ```bash
      dmesg | tail
      ```

5. **Remove the Module**:
    ```bash
    sudo rmmod hello_world
    ```

## Usage

1. **Access the Device File**:
   - Use `cat` or `echo` to interact with `/dev/hello_world`.

2. **Example - Writing to Device**:
    ```bash
    echo "Hello, Kernel!" > /dev/hello_world
    ```

3. **Example - Reading from Device**:
    ```bash
    cat /dev/hello_world
    ```

   These commands will transfer data between user space and the kernel buffer.

## Code Structure

- **hello_world.c**: Main kernel module source file. Defines initialization, cleanup, read, and write operations.
- **Makefile**: Used to compile the module, supporting cross-compilation and kernel header paths.

## Troubleshooting

- **`Operation not permitted`**:
   - Ensure you have root privileges when running `insmod` and `rmmod`.
- **`Device class could not be created`**:
   - This may occur if a previous instance was not unloaded correctly. Use `sudo rmmod hello_world` to unload it, or check `/sys/class/HelloWorldClass` for existing device classes.
- **Cross-Compilation Issues**:
   - Verify that the kernel headers and cross-compilation toolchain match your target environment.

## License

This project is licensed under the **GPL** license - see the `LICENSE` file for details.
