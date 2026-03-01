<div align="center">
  <h1>Pallas</h1>
  <h3>A modern systems language you actually want to use.</h3>
  <br>
  <p>
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

**Pallas** is a systems programming language that combines low-level control with a safer, modern language design. This is a personal project inspired by my friend [Lily](https://github.com/transicle/)'s programming language, [ADAN](https://github.com/cappucina/adan) and many ideas inspired by languages like C/C++, Go, Rust and Zig.

What Pallas gives you:
- Explicit behavior
- Manual memory management
- Structs and classes
- Pattern matching
- Familiar syntax

---

## Community

Pallas is a personal project - Though I'd appreciate any contributions and feedback.

- Discord: **@kauht** or **@yuhbayn**
- Discord Server: https://discord.gg/WVMHUgrgeH
- GitHub: https://github.com/kauht/Pallas

---

## Quick Start

### Prerequisites
- CMake
- A C++17 compatible compiler
- LLVM v14+

### Build

```bash
cmake -S . -B build
cmake --build build
```

### Run

```bash
build/palc --help
```

---

## Language Reference

### Types

Pallas uses explicitly-sized types. No implicit conversions.

| Type | Description |
|:---|:---|
| `i8`, `i16`, `i32`, `i64`, `i128` | Signed |
| `u8`, `u16`, `u32`, `u64`, `u128` | Unsigned |
| `f32`, `f64` | Floating point |
| `bool` | Boolean |
| `char` | Character |
| `string` | String |
| `void` | No Type |

**Type aliases:**
| Alias | Equals |
|:---|:---|
| `int` | `i64` |
| `uint` | `u64` |
| `float` | `f32` |
| `double` | `f64` |

**Pointers and Arrays:**
- `T*` - pointer to T
- `T**` - pointer to pointer
- `T[n]` - array of n size

### Variables and Functions

```pallas
x: i32 = 42;
const PI: f32 = 3.14159;

square(n: i32): i32 {
    return n * n;
}
```

### Structs and Classes

`struct`s hold data only. `class`es support methods, constructors, and destructors.

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

---

## Control Flow

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

### Range-based `for`

Examples:

```pallas
// static array
a: i32[4] = [1, 2, 3, 4];
for (v : a) {
    println("${v}");
}

// dynamic array
v: Vec<i32> = Vec<i32>();
v.push(10);
v.push(20);
for (x : v) {
    println("${x}");
}

for (i: i64 = 0; i < a.size(); i = i + 1) {
    v: i32 = a[i];
    println("${v}");
}
```


---

## Examples

### Simple function

```pallas
add(a: i32, b: i32): i32 {
    return a + b;
}
```

### Using a class

```pallas
main(): void {
    p: Person* = new Person("Sammy", 17);
    p.greet();
    delete p;
}
```

### Generics

```pallas

// Generic class
class Optional<T> {
    has: bool;
    value: T;

    Optional(v: T) { has = true; value = v; }
    ~Optional() { /* code */ }

    is_some(): bool { return has; }
}

// Generic function
identity<T>(x: T): T {
    return x;
}

// Call generic function
n: i32 = identity<i32>(42);

b: Optional<i32> = Optional<i32>(10);
res: Vector<i64> = b.map<i64>((v: i32): i64 { return (i64)v; });

// Type alias
type IntVec = Vector<i32>;

// Default type param
struct SmallVec<T = i32> {
    data: T*;
    len: i64;
}

// Use defaulted generic
sv: SmallVec = SmallVec(); // SmallVec<i32>
```

### Const

`const` can be used for compile-time values. Or non-mutable values that are initialized at runtime, depending on the context.

```pallas
const BUFSZ: i64 = 16;
const NAME: string = "Pallas";
arr: i32[BUFSZ];
```
---

## Roadmap

Goals:

- Finish language spec
- Complete Parser
- Complete Semantic
- Integrate LLVM
- Complete IR
- Complete Codegen
- Add tests for parser, semantics, and codegen
- Write LSP and formatter (VSCode/Zed/Neovim)

---

## Contributing

Contributions are always welcome, check out [CONTRIBUTING.md](CONTRIBUTING.md) for more information.

---

## License

Pallas is MIT licensed - see `LICENSE`.

## Influences

**Pallas is heavily inspired by the following:**
- C, C++, Rust, Zig, Go
- [Lily](https://github.com/transicle/)
- [ADAN](https://github.com/cappucina/ADAN)
- [Zen C](https://github.com/z-libs/Zen-C)

---

## More Documentation

- Design doc and notes: [DESIGN.md](docs/DESIGN.md)
- TO-DO: [TODO.md](docs/TODO.md)
- Examples: [examples/](examples)
