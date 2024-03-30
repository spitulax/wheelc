{
  description = "Some random C project";

  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs, ... }:
  let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
    buildInputs = with pkgs; [
      raylib
    ];
  in
  {
    packages.${system}.default =
      with pkgs; stdenv.mkDerivation {
        pname = "randomahh";
        version = "0.0.1";
        src = ./.;
        inherit buildInputs;
        installPhase = ''
          mkdir -p $out
          cp build/randomahh $out
        '';
      };

    devShells.${system}.default =
      with pkgs; mkShell {
        name = "randomahh-shell";
        inherit buildInputs;
        CPATH = lib.makeSearchPathOutput "dev" "include" buildInputs;
      };
  };
}
