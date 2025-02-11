with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "env";
  nativeBuildInputs = [ pkg-config ];
  buildInputs = [
    SDL2
    SDL2_ttf
    SDL2_gfx
    SDL2_image
    libffi
    zlib
    python3
  ];
}
