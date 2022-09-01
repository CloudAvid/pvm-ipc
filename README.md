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
    $ ./configure
    ```

    > You'd be prompted with **unmet requirements** which **must be** fulfilled before proceeding.

3. After successful `Makefile` creation; let us proceed to `PVM-IPC` compilation and installation.
      - At this point (specificity *installation* part) you need `superuser` privilege.

    ```bash
    # Compile the project.
    $ make
    # If you like, we could install it as well
    $ make install
    ```

> Use pkg-config to discover the necessary include and linker arguments. Issue this:

```bash
# Displays pparam necessary liker and compile flags.
pkg-config libpparam-1.0 --cflags –libs
```

## Documentation

Read the project [wiki](https://github.com/CloudAvid/pvm-ipc/wiki).

## Contribution and Bug reports

Please check [contribution](CONTRIBUTING.md) standard for full explanation.

## About

This project brought you by [CloudAvid](https://www.cloudavid.com) developer team.
