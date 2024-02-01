with import <nixpkgs> {};
mkShell {
	nativeBuildInputs = [ 
		(python3.withPackages(ps: with ps; [ pillow]))
		gcc-arm-embedded-13
		cmake
		openocd-rp2040
		(import ./64tass.nix)
		SDL2
		zip
		pkgsCross.mingwW64.buildPackages.gcc
	];
	# PICO_SDK_PATH = "<full path to pico sdk>";
}
