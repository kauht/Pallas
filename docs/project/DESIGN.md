# Pallas Language Design

## Type System

### No Implicit Casting

Pallas enforces explicit type casting.

```pallas
x: i32 = 5;
y: f32 = x;         // ERROR: cannot implicitly cast i32 to f32
y: f32 = (f32)x;    // OK: explicit cast
```

### Built-in Types

#### Default Types
- `int` - Integer (platform-dependent size)
- `float` - Single precision floats
- `double` - Double precision floats  
- `char` - Character
- `string` - String type
- `bool` - Boolean
- `void` - No return type

#### Sized Types
- **Signed integers**: `i8`, `i16`, `i32`, `i64`
- **Unsigned integers**: `u8`, `u16`, `u32`, `u64`
- **Floating point**: `f8`, `f16`, `f32`, `f64`

### Pointers

```pallas
ptr: int*; // Pointer to int

ptr_to_ptr: i32**; // Pointer to pointer to i32

// Null pointers
ptr: i32* = null;
```

---

## Classes and Structs

### Structs

Structs are simple data containers:

```pallas
struct Point {
    x: i32;
    y: i32;
};

struct Vector3 {
    x: f32;
    y: f32;
    z: f32;
};
```

### Classes

Classes support methods, constructors, destructors, and access control:

```pallas
class Person {
    public:
    name: string;
    age: i32;
    
    // Constructor - same name as class
    Person(n: string, a: i32) {
        name = n;
        age = a;
    }
    
    // Destructor - prefixed with ~
    ~Person() {
        // Cleanup code
    }
    
    greet(): void {
        println("Hello, I'm {}", name);
    }
    
    private:
    ssn: string;
    
    validate(): bool {
        return age >= 0;
    }
};
```

### Access Modifiers

- `public:` - Accessible from anywhere (applies to all following members)
- `private:` - Only accessible within the class (applies to all following members)

Like C++, visibility labels apply to all members that follow until another label is encountered.

```pallas
class BankAccount {
    public:
    deposit(amount: f32): void {
        balance += amount;
    }
    
    private:
    balance: f32;
    account_number: string;
};
```

### Constructors and Destructors

**Constructors** initialize objects when created:
- Same name as the class
- No return type
- Can have parameters

**Destructors** clean up when objects are destroyed:
- Class name prefixed with `~`
- No parameters
- No return type
- Called automatically when object goes out of scope

```pallas
class Resource {
    // Constructor
    Resource(filename: string) {
        println("Opening {}", filename);
        // Initialize resource
    }
    
    // Destructor
    ~Resource() {
        println("Closing resource");
        // Clean up
    }
};
```

### Object Instantiation

```pallas
// Stack allocation
person: Person = Person("Alice", 30);

// With new (heap allocation)
Person* ptr = new(Person);

// Method calls
person.greet();
ptr->greet();
```

---

## Memory Management

### Manual Control

Like C, you are responsible for memory management:
- No garbage collection
- Explicit allocation and deallocation
- Direct control over memory layout

### Allocation and Deallocation

```pallas
// Allocate memory
ptr: i32* = new(i32);
array: i32* = new(i32[10]);  // Array allocation

// Use the memory
*ptr = 42;

// Free memory when done
delete(ptr);
delete(array);
```

### Stack vs Heap

```pallas
// Stack allocation (automatic cleanup)
x: i32 = 5;
person: Person = Person("Bob", 25);

// Heap allocation (manual cleanup required)
ptr: i32* = new(i32);
obj: Person* = new(Person);

delete(ptr);
delete(obj);
```

---

## Syntax Standards

### Variable Declarations

```pallas
// Basic declaration with initialization
name: string = "Alice";
age: i32 = 30;

// Declaration without initialization
height: f32;

// Constant variables (immutable)
const PI: f32 = 3.14159;
const MAX_SIZE: i32 = 100;
```

### Type Annotations

All variables must have explicit type annotations:

```pallas
x: i32 = 42;        // OK
y := 42;            // ERROR: type inference not supported (yet)
```

---

## Control Flow

### If-Else Statements

```pallas
if (condition) {
    // code
} else if (other_condition) {
    // code
} else {
    // code
}
```

### Loops

#### For Loop (C-style)

```pallas
for (i: i32 = 0; i < 10; i++) {
    println("{}", i);
}
```

#### While Loop

```pallas
while (condition) {
    // code
    if (should_exit) {
        break;
    }
    if (should_skip) {
        continue;
    }
}
```

#### Future Loop Variants (Under Consideration)

Range-based for loop:
```pallas
// Proposed syntax
for (i in 0..10) {
    println("{}", i);
}
```

Iterator for loop:
```pallas
// Proposed syntax
for (item : array) {
    println("{}", item);
}
```

---

## Functions

### Function Declaration

```pallas
// Basic function
add(a: i32, b: i32): i32 {
    return a + b;
}

// Void function (no return value)
greet(name: string): void {
    println("Hello, {}", name);
}

// No parameters
get_random(): i32 {
    return 42;
}
```

### Variadic Functions

Functions that accept a variable number of arguments:

```pallas
println(format: string, ...): void {
    // Implementation accesses variadic arguments
    // Exact mechanism TBD
}

// Usage
println("Hello, {}!", "World");
println("Values: {}, {}, {}", 1, 2, 3);
```

### Function Parameters

```pallas
// Pass by value (default)
increment(x: i32): i32 {
    return x + 1;
}

// Pass by pointer (for modification)
increment_ref(x: i32*): void {
    *x = *x + 1;
}
```

---

## Module System

### Import Statements

```pallas
// Import entire module
import std.io;
import std.collections;

// Include specific file (alternative syntax)
include something.cool;

// Usage
std.io.println("Hello");
```

### Future Import Variants (Proposed)

```pallas
// Import specific items
from std.io import println, print;

// Import with alias
import std.collections as col;
```

---

## Operators

### Arithmetic Operators

```pallas
+   // Addition
-   // Subtraction
*   // Multiplication
/   // Division
%   // Modulo
++  // Increment
--  // Decrement
```

### Assignment Operators

```pallas
=   // Assignment
+=  // Add and assign
-=  // Subtract and assign
*=  // Multiply and assign
/=  // Divide and assign
```

### Comparison Operators

```pallas
==  // Equal to
!=  // Not equal to
<   // Less than
<=  // Less than or equal to
>   // Greater than
>=  // Greater than or equal to
```

### Logical Operators

```pallas
&&  // Logical AND
||  // Logical OR
!   // Logical NOT
```

### Bitwise Operators

```pallas
&   // Bitwise AND
|   // Bitwise OR
^   // Bitwise XOR
~   // Bitwise NOT
<<  // Left shift
>>  // Right shift
```

### Compound Bitwise Assignment

```pallas
&=  // AND and assign
|=  // OR and assign
^=  // XOR and assign
<<= // Left shift and assign
>>= // Right shift and assign
```

### Member Access Operators

```pallas
.   // Member access (object.member)
->  // Pointer member access (ptr->member)
```

### Other Operators

```pallas
?:  // Ternary operator (proposed)
[]  // Array subscript
()  // Function call
```

---

## Examples

### Complete Program

```pallas
class Counter {
    public:
    Counter(initial: i32) {
        count = initial;
    }
    
    increment(): void {
        count++;
    }
    
    get_count(): i32 {
        return count;
    }
    
    private:
    count: i32;
};

main(): i32 {
    counter: Counter = Counter(0);
    
    for (i: i32 = 0; i < 5; i++) {
        counter.increment();
    }
    
    println("Count: {}", counter.get_count());
    
    return 0;
}
```

### Memory Management Example

```pallas
class Buffer {
    public:
    Buffer(size: i32) {
        data = new(u8[size]);
        length = size;
    }
    
    ~Buffer() {
        delete(data);
    }
    
    get(index: i32): u8 {
        return data[index];
    }
    
    private:
    data: u8*;
    length: i32;
};

main(): i32 {
    buffer: Buffer(256);
    // Buffer automatically cleaned up when out of scope
    return 0;
}
```

---

## Philosophy

Pallas is designed to be:

1. **Explicit**: No surprises, no implicit conversions
2. **Powerful**: Low-level control when needed
3. **Safe**: Modern features to prevent common bugs
4. **Familiar**: Syntax inspired by C/C++ and Rust
5. **Optional OOP**: Classes are tools, not requirements

---

## Future Considerations

Features under consideration for future versions:

- **Generics/Templates**: Parameterized types for reusable code
- **Pattern Matching**: Advanced switch/case statements
- **Lambdas**: Anonymous functions for functional programming
- **Move Semantics**: Efficient resource transfer
- **Compile-time Execution**: Constexpr-like features
- **Macros**: Compile-time code generation

---

**Last Updated**: December 2024
