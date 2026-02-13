<div align="center">
  <h1>Pallas</h1>
  <h3>A modern systems language you actually want to use.</h3>
  <br>
  <p>
    <a href="#"><img src="https://github.com/kauht/Pallas/actions/workflows/cmake-multi-platform.yml/badge.svg" alt="Build Status"></a>
    <a href="#"><img src="https://img.shields.io/badge/license-MIT-blue" alt="License"></a>
    <a href="#"><img src="https://img.shields.io/badge/platform-x64-lightgrey" alt="Platform"></a>
  </p>
  <p><em>Don't sacrifice beauty for control.</em></p>
</div>

<hr>

<div align="center">
  <p>
    <b><a href="#overview">Overview</a></b> •
    <b><a href="#community">Community</a></b> •
    <b><a href="#quick-start">Quick Start</a></b> •
    <b><a href="#language-reference">Language Reference</a></b> •
    <b><a href="#examples">Examples</a></b> •
    <b><a href="#roadmap">Roadmap</a></b>
  </p>
</div>

---

## Overview

**Pallas** is a personalized systems programming language that combines low-level control with a safer, modern language design. This is a personal project inspired by my friend [Lily](https://github.com/transicle/)'s programming language, [ADAN](https://github.com/cappucina/adan) and many ideas inspired by successful languages like C/C++, Go, Rust and Zig.

The goal of pallas is to give you a healthy mix of safety and control without compromise, and zero-cost semantic, syntax, and functional abstractions. The syntax is familiar and simple, easy to pick up if you know other C-style languages.

What Pallas gives you:
- Explicit behavior (no implicit conversions and predictable semantics)
- Manual memory management and powerful stdlib helpers
- Structs and classes
- Pattern matching and C-style control flow
- Readable, familiar syntax

---

## Community

Pallas is a personal project - I'd appreciate contributions, feedback, and discussion.

- Personal Discord: ***@kauht*** or ***@yuhbayn***

- Discord: [Server](https://discord.gg/WVMHUgrgeH)
- GitHub: [kauht/Pallas](https://github.com/kauht/Pallas)

---

## Index

<table align="center">
  <tr>
    <th width="50%">General</th>
    <th width="50%">Language Reference</th>
  </tr>
  <tr>
    <td valign="top">
      <ul>
        <li><a href="#overview">Overview</a></li>
        <li><a href="#community">Community</a></li>
        <li><a href="#quick-start">Quick Start</a></li>
        <li><a href="#examples">Examples</a></li>
        <li><a href="#roadmap">Roadmap</a></li>
        <li><a href="#contributing">Contributing</a></li>
        <li><a href="#license">License</a></li>
      </ul>
    </td>
    <td valign="top">
      <ul>
        <li><a href="#types">Types</a></li>
        <li><a href="#variables-and-functions">Variables & Functions</a></li>
        <li><a href="#structs-and-classes">Structs & Classes</a></li>
        <li><a href="#control-flow">Control Flow</a></li>
        <li><a href="#pattern-matching">Pattern Matching</a></li>
        <li><a href="#pointers-and-memory">Pointers & Memory</a></li>
        <li><a href="#type-casting">Type Casting</a></li>
      </ul>
    </td>
  </tr>
</table>

---

## Quick Start

### Prerequisites
- CMake
- A C11-compatible compiler (clang / gcc / MSVC)
- LLVM v14+

### Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -- -j
```

### Run

```bash
cmake --build build --target run

build/palc --help
```

---

## Language Reference

### Types

Pallas uses explicitly-sized types. No implicit numeric conversions.

| Type | Description |
|:---|:---|
| `i8`, `i16`, `i32`, `i64`, `i128` | Signed integers |
| `u8`, `u16`, `u32`, `u64`, `i128` | Unsigned integers |
| `f32`, `f64` | Floating point |
| `bool` | Boolean |
| `char` | Single character |
| `string` | String type |
| `void` | Empty type |

**Type aliases:**
| Alias | Maps to |
|:---|:---|
| `int` | `i64` |
| `uint` | `u64` |
| `float` | `f32` |
| `double` | `f64` |

**Pointers and Arrays:**
- `T*` - pointer to T
- `T**` - pointer to pointer
- `T[n]` - fixed-size array

### Variables and Functions

```pallas
x: i32 = 42;
const PI: f32 = 3.14159;

square(n: i32): i32 {
    return n * n;
}
```

### Structs and Classes

**Structs** are data-only. **Classes** support methods, constructors, and destructors.

```pallas
struct Vector3 {
    x: f32;
    y: f32;
    z: f32;
}

class Person {
    public:
        name: string;
        age: i32;

        Person(n: string, a: i32) {
            name = n;
            age = a;
        }

        ~Person() {
            // destructor
        }

        greet(): void {
            println("Hello, ${name}, you are ${age} years old!");
        }

    private:
        ssn: string;
}
```

### Control Flow

```pallas
if (x > 10) {
    println("large");
} else {
    println("small");
}

for (i: i32 = 0; i < 10; i++) {
    println("${i}");
}

counter: i32 = 0;
while (counter < n) {
    counter++;
}
```

### Pattern Matching

```pallas
match (x) {
    0 => { println("zero"); }
    1 => { println("one"); }
    42 => { println("the answer"); }
    _ => { println("something else"); }
}
```

### Pointers and Memory

Manual memory management with `new` and `delete`.

```pallas
x: i32 = 5;
p: i32* = &x;
*p = 10;

arr: i32* = new i32[10];
arr[0] = 1;
delete arr;
```

### Type Casting

No implicit conversions between integers and floats, besides when using literals.

```pallas
x: i32 = 5;
y: f32 = (f32)x;
```

---

## Examples

### Function

```pallas
add(a: i32, b: i32): i32 {
    return a + b;
}
```

### Using a Class

```pallas
main(): void {
    p: Person* = new Person("Sammy", 17);
    p.greet();
    delete p;
}
```

---

## Roadmap

Goals:

- Finish and publish language spec
- Start C++ Port
- Complete Lexer
- Complete Parser
- Complete Semantic
- Complete IR
- Complete Codegen
- Add tests for parser, semantics, and codegen
- Write LSP and formatter (VSCode/Zed/Neovim Plugins)

---

## Contributing

Contributions are always welcome, check out [CONTRIBUTING.md](CONTRIBUTING.md) for more information.

---

## License

Pallas uses MIT licensed - see `LICENSE`.

## Influences

**Pallas is heavily inspired by the following projects**
- C, C++, Rust, Zig, Go
- [Lily](https://github.com/transicle/)
- [ADAN](https://github.com/cappucina/ADAN)
- [Zen C](https://github.com/z-libs/Zen-C)

---

## More Documentation

- Design doc and notes: [DESIGN.md](docs/DESIGN.md)
- TO-DO: [TODO.md](docs/TODO.md)
- Examples: [examples/](examples)
