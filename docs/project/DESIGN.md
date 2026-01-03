# Pallas Language Design

## Type System

### No Implicit Casting

```c
x: f32 = 5;
y: f32 = x;         // ERROR
y: f32 = (f32)x;    // OK
```

### Types

#### Default Types
- `int` - Integer
- `float` - Single precision floats
- `double` - Double precision floats  
- `char` - Character
- `bool` - Boolean

#### Sized Types
- `i8`, `i16`, `i32`, `i64` - Signed integers
- `u8`, `u16`, `u32`, `u64` - Unsigned integers
- `f32`, `f64` - Floats

### Pointers

```c
int* ptr;       // Pointers like C
i32* a, b, c;   // ALL 3 ARE POINTERS
```

## Classes/Structs

```c
struct Point {
    int x;
    int y;
}; // Like C++

// Class with methods
class Vector3 {
public:
    float x, y, z;
private:
    float m() {
    }
};
```

Pallas is NOT an object oriented programming language. The language is not built off of them, they are to be used as a tool and are completely optional.

## Memory Management

### Control
Just like in C, you're responsible for managing memory:
- allocation and deallocation
- no garbage collection

## Syntax Standards

### Variables
```c
a: i32 = 1;
const b: i32 = 0; // Constant variables
```

### Functions
```c
main(): int {
    // function body
}
```

### Control Flow
```c
if (cond) {
} else if (other) {
} else {
}

// Loops
for (int i = 0; i < n; i++) {
}

while (cond) {
}

// Ranged for???? UNSURE
for (i: i32 in 0..10) {
    println("{}", i);
}
// Or just
for (i: i32 = 0; i < 10; i++) {

}

// Iterator for
for (item: i32 : array) {
    println("{}", item);
}
```

### Variadic functions
```c
println(fmt: str, ...) {
    // Accessing variadic arguments: UNSURE
}
```

### Library System
```c
import std.io;
import std.collections;

// Maybe
from std.io import println;
import std.collections as col;
```

### Memory Allocation/Deallocation
```c
p: i32* = new(i32);

free(p);
```
