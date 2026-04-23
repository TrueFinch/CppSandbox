# Build Instructions

Always use the defined CMake presets to ensure consistent builds between CLion and the CLI.

## Workflow

### 1. Configuration

To configure the project with the `DebugPreset` preset (includes `vcpkg` toolchain and `BUILD_DEMO=ON`):

```bash
cmake --preset DebugPreset
```

### 2. Building Targets

Use the `--target` flag to build only the specific project you are working on. This prevents unnecessary builds of other
sub-projects.

**Build a specific target (e.g., Lambda):**

```bash
cmake --build --preset DebugPreset --target Lambda
```

**Build a specific target (e.g., MyAny):**

```bash
cmake --build --preset DebugPreset --target MyAny
```

**Build everything:**

```bash
cmake --build --preset DebugPreset
```

## Notes

- All builds are directed to the `cmake-build-debug` directory.
- The `vcpkg` toolchain and `x64-mingw-dynamic` triplet are pre-configured in the preset.
