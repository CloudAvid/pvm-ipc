# PVM Interprocess Communication Library

- [PVM Interprocess Communication Library](#pvm-interprocess-communication-library)
  - [Introduction](#introduction)
  - [Installation](#installation)
  - [Documentation](#documentation)
  - [Contribution and Bug reports](#contribution-and-bug-reports)
  - [About](#about)

## Introduction

**PVM-IPC** provides users with sets of easy-to-use *Interprocess Communication* methods based on **OOP** approaches.

## Installation

Installation of `PVM-IPC` is based on `Autoconf` and `Autotools` build system.
> Therefore, make sure they are installed properly before you proceed.

1. First step is to clone the project in desired directory:

    ```bash
    # Your favorite directory
    $ git clone https://github.com/CloudAvid/pvm-ipc.git
    ```

2. Then for `Makefiles` creation issue these commands at cloned directory.

    ```bash
    # Creates Configuration files.
    $ ./autogen.sh

    # Creates Makefiles for building project.
    # to enable examples compilation, add `--enable-examples` to end of `./configure`
    $ ./configure
    ```

    > You'd be prompted with issues with installed or not-installed packages, resolve these issues then move on.

3. After successful `Makefile` creation; let us proceed to `PVM-IPC` compilation and installation.
      - At this point (specificity *installation* part) you need `superuser` privilege.

    ```bash
    # Compile the project using multiple jobs.
    $ make -j8
    # To install the package first we need to create RPM package.
    $ make rpm
    # Then install the RPM packages with:
    $ make install_rpm
    ```

> Use pkg-config to discover the necessary include and linker arguments. Issue this:

```bash
# Displays PVM_IPC necessary liker and compile flags.
pkg-config --cflags --libs pvm-ipc
```

## Documentation

Read the project [wiki](https://github.com/CloudAvid/pvm-ipc/wiki).

## Contribution and Bug reports

Please check [contribution](CONTRIBUTING.md) standard for full explanation.

## About

This project brought you by [CloudAvid](https://www.cloudavid.com) developer team.
