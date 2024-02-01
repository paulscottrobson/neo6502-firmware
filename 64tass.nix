with import <nixpkgs> {};

stdenv.mkDerivation rec {
  pname = "64tass";
  version = "1.58.2974";

  src = pkgs.fetchgit {
    url = "https://github.com/irmen/64tass.git";
    rev = "V1.58.2974";
    sha256 = "sha256-72UYHWSBRl1bFLep2MpGXdGwxPOEKPKt8yu1X+tz/Hc=";
  };

  buildInputs = [
    pkgs.gcc
    pkgs.gnumake
  ];

  buildPhase = ''
    make
  '';

  installPhase = ''
    mkdir -p $out/bin
    mv 64tass $out/bin
  '';
}
