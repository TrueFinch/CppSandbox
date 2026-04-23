# Agent Instructions for TypePrinter Project

## Core Requirement
The primary goal of this project is to implement a template-based mechanism for printing type names as strings.

## Constraints
- **NO `typeid`**: Do NOT use `typeid` or any other compiler-intrinsic/built-in mechanisms for retrieving type names.
- **NO Compiler-Specific Intrinsics**: Any feature that relies on compiler-specific magic to "know" the name of a type is strictly forbidden.

## Type Printing Logic
- **Primitive Types**: Must be mapped to their corresponding string representation (e.g., `int` -> `"int"`, `float` -> `"float"`) via explicit mapping or traits.
- **Custom Classes/Structs**: Must use the `static std::string_view name` (or similar) defined within the class.
- **Complex Types**:
  - **Pointers**: Must correctly reflect `*` and `const` (e.g., `int* const`).
  - **References**: Must correctly reflect `&` and `&&`.
  - **Functions**: Must resolve function signatures (e.g., `R(Args...)`).
  - **Containers**: Must resolve container types (e.g., `std::vector<T>`).
- **Fallback**: If a type has no defined name or recognizable structure, return `"no data"` or a similar placeholder.
