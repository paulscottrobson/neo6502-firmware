#!/bin/sh

# Hello World assembly example for the Neo6502 (build script)
# SPDX-License-Identifier: CC0-1.0
#
# requires '64tass' assembler


# environment checks
which 64tass > /dev/null || ! echo "could not find '64tass' program" || exit 1


# cleanup
rm -f hello.lst hello.neo

# assemble
64tass --mw65c02 --nostart --list hello.lst --output=hello.neo hello.asm

# launch emulator
test -f hello.neo && ../../bin/neo hello.neo@800 cold
