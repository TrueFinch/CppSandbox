# Build Instructions

Always use the defined CMake presets to ensure consistent builds between CLion and the CLI.

## Workflow

### 1. Configuration
To configure the project with the `debug` preset (includes `vcpkg` toolchain and `BUILD_DEMO=ON`):
```bash
cmake --preset debug
```

### 2. Building Targets
Use the `--target` flag to build only the specific project you are working on. This prevents unnecessary builds of other sub-projects.

**Build a specific target (e.g., Lambda):**
```bash
cmake --build --preset debug --target Lambda
```

**Build a specific target (e.g., MyAny):**
```bash
cmake --build --preset debug --target MyAny
```

**Build everything:**
```bash
cmake --build --preset debug
```

## Notes
- All builds are directed to the `cmake-build-debug` directory.
- The `vcpkg` toolchain and `x64-mingw-dynamic` triplet are pre-configured in the preset.
