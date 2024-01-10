if [[ -z "${PICO_SDK_PATH}" ]]; then
  export PICO_SDK_PATH=/usr/share/pico_sdk
  echo "PICO_SDK_PATH not set, trying PICO_SDK_PATH=${PICO_SDK_PATH}"
else
  echo "PICO_SDK_PATH=${PICO_SDK_PATH}"
fi

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

