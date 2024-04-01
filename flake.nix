{
  description = "We'll see";

  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs, ... }:
  let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
  in
  {
    packages.${system}.default =
      with pkgs; stdenv.mkDerivation {
        pname = "wheelc";
        version = "0.0.1";
        src = ./.;
        buildInputs = [
          raylib
        ];
        installPhase = ''
          mkdir -p $out/bin
          cp build/wheelc $out/bin
        '';
      };

    devShells.${system}.default =
      with pkgs; self.packages.${system}.default.overrideAttrs (finalAttrs: previousAttrs: {
        CPATH = lib.makeSearchPathOutput "dev" "include" finalAttrs.buildInputs;
      });
  };
}
