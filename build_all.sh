if [[ -z "${PICO_SDK_PATH}" ]]; then
  export PICO_SDK_PATH=/usr/share/pico_sdk
  echo "PICO_SDK_PATH not set, trying PICO_SDK_PATH=${PICO_SDK_PATH}"
else
  echo "PICO_SDK_PATH=${PICO_SDK_PATH}"
fi


#6502 software
make -C software/apple1-basic
make -C software/fig-forth

#kernel
make -C kernel



#prepare firmware
rm -Rf firmware/build
mkdir -p firmware/build
cmake -B firmware/build firmware
make -C firmware


make -C emulator

make
