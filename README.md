# Sweep'n Time

## Project Directory

```text
|-- assets/                   # Game assets (sprites, textures, audio, fonts)
|-- lib/                      # External libraries directly copied from source. These should rarely be modified.
|-- platforms/                # Platform-specific build wrappers and launcher code
|-- src/                      # Core game source code and logic
|   |-- main.c                # Main file. This is where most of the initialization takes place, where the main game loop resides and where most of the cleanup happens.
|   |-- assets.c              # Asset loading and management. This includes colors, fonts, etc.
|   |-- config.h              # Configuration parameters for the game, including constants that shouldn't be modified.
|   |-- components.h          # Structs for all the ECS components
|   `-- systems/              # ECS systems
|-- CMakeLists.txt            # CMake build file
|-- LICENSE.md                # License file
|-- README.md                 # This file
|-- WORKSPACE.code-workspace  # VSCode workspace file
|-- asan_suppressions.txt     # AddressSanitizer suppressions file
|-- flake.lock                # Nix flake lock file
|-- flake.nix                 # Nix flake file
`-- justfile                  # Justfile for building and running the game. This is like the control pannel for this project.
```

---

## 📄 Licensing & Copyright

*Sweep'n Time* is a source-available project. To balance community collaboration with commercial protection, this project operates under a split-licensing model:

* **Source Code:** Managed under the **PolyForm Noncommercial License 1.0.0** (allows personal use and modifications, strictly prohibits commercial use).
* **Game Assets:** Managed under **Creative Commons Attribution-NonCommercial-ShareAlike 4.0** (allows modding and remixing for non-commercial use).
* **Branding:** The game title, logos, and associated trademarks are fully reserved.

Please read our master [LICENSE.md](LICENSE.md) file for the full, binding legal terms before building, modding, or sharing this project.

---

## 🤝 How to Contribute (CLA Requirement)

We welcome bug fixes and performance optimizations! Because we plan to bring *Sweep'n Time* to commercial storefronts in the future, **all contributors must sign our Contributor License Agreement (CLA)** before a Pull Request can be merged.

1. Submit your Pull Request.
2. Our automated **CLAssistant Bot** will reply with instructions.
3. Simply comment back with the exact phrase:
   > `I have read the CLA Document and I hereby sign the CLA`
4. The bot will instantly greenlight your PR for review!
