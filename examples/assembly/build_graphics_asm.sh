#!/bin/sh

# Graphics assembly example for the Neo6502 (build script)
# SPDX-License-Identifier: CC0-1.0
#
# requires '64tass' assembler


# environment checks
which 64tass > /dev/null || ! echo "could not find '64tass' program" || exit 1


cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

# cleanup
rm -f graphics.lst graphics.neo

# assemble
64tass --mw65c02 --nostart --list graphics.lst --output=graphics.neo graphics.asm

# launch emulator
test -f graphics.neo && ../../bin/neo graphics.neo@800 cold

cd -
