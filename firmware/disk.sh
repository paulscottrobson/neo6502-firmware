umount /dev/sde1
mkfs.fat -F32 /dev/sde1
mkdir -p /tmp/h
mount /dev/sde1 /tmp/h
cp /home/paulr/Projects/neo6502-firmware/basic/storage/* /tmp/h
umount /dev/sde1