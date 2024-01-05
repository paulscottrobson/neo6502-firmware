if [[ -z "${PICO_SDK_PATH}" ]]; then
  export PICO_SDK_PATH=/usr/share/pico_sdk
  echo "PICO_SDK_PATH not set, trying PICO_SDK_PATH=${PICO_SDK_PATH}"
else
  echo "PICO_SDK_PATH=${PICO_SDK_PATH}"
fi

pushd firmware/libraries
git submodule update --init -- fatfs PicoDVI tinyusb
popd

make all

