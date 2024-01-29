pushd firmware/libraries
git submodule update --init -- no-OS-FatFS-SD-SPI-RPi-Pico
popd

pushd emulator
make clean
make 
popd

pushd basic
make test
popd basic

make all

