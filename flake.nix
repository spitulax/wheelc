{
  description = "Some random C project";

  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs, ... }:
  let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
  in
  {
    packages.${system}.default =
      with pkgs; stdenv.mkDerivation (finalAttrs: {
        pname = "randomahh";
        version = "0.0.1";
        src = ./.;
        buildInputs = [
          raylib
        ];
        installPhase = ''
          mkdir -p $out
          cp build/randomahh $out
        '';
      });

    devShells.${system}.default =
      with pkgs; mkShell {
        name = "randomahh-shell";
        inputsFrom = [ self.outputs.packages.${system}.default ];
      };
  };
}
