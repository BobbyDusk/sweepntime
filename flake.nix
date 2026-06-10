{
  description = "Development environment for nestled. A C/C++ environment using the clang toolchain.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/release-26.05";   
    flake-utils.url = "github:numtide/flake-utils";
    nixgl.url = "github:nix-community/nixGL";
  };

  outputs = { self, nixpkgs, flake-utils, nixgl }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        # No complex overlay management required for standalone apps
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          name = "cnestled-dev-shell";
          buildInputs = with pkgs; [
            # Clang toolchain
            clang # compiler
            llvm 
            lld # linker

            # Build system
            cmake # build system
            ninja # build system

            # Debugging tools & profiling
            lldb # debugger
            valgrind # memory profiler
            gperftools # CPU profiler

            # Additional tools
            pkg-config # for finding libraries
            clang-tools # clang-tidy, clang-format, clangd, etc.
                        # clangd provides language server features for code completion, navigation, and refactoring in supported editors.
                        # clang-tidy is a linter and static analysis tool that helps identify potential issues in C/C++ code.
                        # clang-format is a code formatter that enforces consistent coding style.
            ccache # compiler cache

            # Libraries
            sdl3 # for multimedia and game development
            sdl3-image # for image loading
            sdl3-mixer # for audio
            sdl3-ttf # for text rendering

            # Pull nixGL directly from its flake packages definition
            nixgl.packages.${system}.default

            # Additional
            act # for running GitHub Actions locally
            just # a command runner
          ];

          shellHook = ''
            export CC=clang
            export CXX=clang++
            echo "Environment loaded. Run your app using: nixGL ./build/nestled"
          '';
        };
      }
    );
}