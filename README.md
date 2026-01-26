<div align="center">

# Pallas

Pallas is a general purpose and systems programming language written in C++.

Named after Pallas (the epithet of Athena), the goddess of wisdom, craft, and strategy.

</div>

> [!WARNING]
> **Pallas is currently in early development.**

## What it is
Pallas gives you the low-level control of C while, implemented with a safer, modern design. This is a personal project inspired by my friend [Lily](https://github.com/transicle/)'s programming language, [ADAN](https://github.com/cappucina/adan) and many ideas taken from successful languages like C/C++, Rust and GO.

## Quick build

```bash
# Default (Debug)
make

# Release build
make build-release

# Debug build
make build-debug
```

## Quick run

```bash
# Run the Release build
make run

# Run the Debug build
make debug

# Or run directly:
./build/release/pallas --help
```

## Status / features
- Explicit sized types, no implicit casts.
- Structs/classes, manual new/delete with destructors.

## Docs
See [docs/project/DESIGN.md](docs/project/DESIGN.md)

## License
Open source - see [LICENSE](LICENSE)
