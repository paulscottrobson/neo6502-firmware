#!/bin/sh

# Melody assembly example for the Neo6502 (build script)
# SPDX-License-Identifier: CC0-1.0
#
# requires '64tass' assembler


# environment checks
which 64tass > /dev/null || ! echo "could not find '64tass' program" || exit 1


# cleanup
rm -f melody.lst melody.neo

# assemble
64tass --mw65c02 --nostart --list melody.lst --output=melody.neo melody.asm

# launch emulator
test -f melody.neo && ../../bin/neo melody.neo@800 cold
