{ pkgs ? import (fetchTarball "https://github.com/NixOS/nixpkgs/archive/20f65b86b6485decb43c5498780c223571dd56ef.tar.gz") {}}:
pkgs.mkShell {
	nativeBuildInputs = with pkgs.buildPackages; [ 
		(python3.withPackages(ps: with ps; [ pillow gitpython ]))
		gcc-arm-embedded-13
		cmake
		openocd-rp2040
		(import ./64tass.nix)
		SDL2
		zip
		pkgsCross.mingwW64.buildPackages.gcc
	];
	PICO_SDK_PATH = "/home/sash/Code/neo6502/neo6502-firmware/pico-sdk";
}
