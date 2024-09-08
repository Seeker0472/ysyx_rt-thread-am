{
  description = "ysyx_workbench";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs,flake-utils }: 
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        my-verilator= (pkgs.verilator.overrideAttrs (oldAttrs: rec {
          version = "5.008"; 
          src = pkgs.fetchFromGitHub {
            owner = "verilator";
            repo = "verilator";
            rev = "v${version}";
            sha256 = "0wqnn2r8kjbrm28c5qajldw5axmrkslds1957wz9r4qfyhd43qpr";
          };
          patches = []; # 禁用所有补丁
        }));
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            # 指定verilator版本为5.008
            my-verilator
            #build-essential
            pkgs.man
            #gcc-doc
            pkgs.git
            pkgs.gdb
            pkgs.readline #libreadline-dev
            pkgs.vim
            pkgs.gcc
            pkgs.python3
            pkgs.SDL2
            pkgs.SDL2_image
            pkgs.SDL2_ttf
            # verilator start
            pkgs.perl
            pkgs.flex
            pkgs.bison
            pkgs.ccache
            pkgs.gperftools
            pkgs.numactl
            # verilator end
            pkgs.llvm_18
            pkgs.mill
            pkgs.scons
            pkgs.pkgsCross.riscv64.buildPackages.gcc
            pkgs.pkgsCross.riscv64.buildPackages.binutils
            # TODO:add more!!!
            # pkgs.ncurses6
            pkgs.ncurses
          ];
          
          NIX_LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
            # 添加需要的库
            pkgs.ncurses
            pkgs.readline
            pkgs.SDL2
            pkgs.SDL2_image
            pkgs.SDL2_ttf
            pkgs.libz
            # pkgs.llvmPackages_latest.llvm
          ];
          NIX_LD = pkgs.lib.fileContents "${pkgs.stdenv.cc}/nix-support/dynamic-linker";
        };
      }
    );
}
