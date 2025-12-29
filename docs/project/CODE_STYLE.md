## Naming conventions

- Functions and variables: `snake_case`.
- Types: `PascalCase`.
- Macros/constants: `UPPER_SNAKE_CASE`.
- Use consistent and descriptive names

## Code style

- Use `.clang-format`
- Indentation: 4 spaces
- Always use braces, even single line structures
- No implicit conversions (always cast explicitly)

## Error handling

- Define a small enum of error codes in a central header (e.g., `PALLAS_SUCCESS`, `PALLAS_ERROR`, `PALLAS_INIT_ERROR`, `PALLAS_COMPILE_ERROR`).
- Keep diagnostics (file, line, column, severity, message) separate and machine-friendly where possible.

---


## Logging/Errors and diagnostics

- Use logging (`log(level, fmt, ...)`).
- Use errors (`error(level, line, collumn, fmt, ...)`)

---

## PR/Commit etiquette

- Keep commits small and focus on a specific task(I never follow this rule).
- Add short description and reason for design decisions/changes
- Run `make format` before committing anything

---
