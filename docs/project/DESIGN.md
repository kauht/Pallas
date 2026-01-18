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
- `int` - Integer
- `float` - Single precision floats
- `double` - Double precision floats  
- `char` - Character
- `string` - String type
- `bool` - Boolean
- `void` - No return type

#### Sized Types
- **Signed integers**: `i8`, `i16`, `i32`, `i64`
- **Unsigned integers**: `u8`, `u16`, `u32`, `u64`
- **Floating point**: `f32`, `f64`

### Pointers and Arrays

```pallas
ptr: int*;              // Pointer to int
ptr_to_ptr: i32**;      // Pointer to pointer to i32
arr: i32[10];           // Fixed-size array

// Null pointers
ptr: i32* = null;
```

### Type Syntax

```pallas
i32        // Base type
i32*       // Pointer to i32
i32**      // Pointer to pointer
i32[10]    // Array of 10 i32s
i32*[5]    // Array of 5 pointers to i32
```

---

## Classes and Structs

### Structs

Structs are just data:

```pallas
struct Vector3 {
    x: f32;
    y: f32;
    z: f32;
}
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
            println("Hello, I'm ${name}");
        }
    
    private:
        ssn: string;
        
        validate(): bool {
            return age >= 0;
        }
}
```

### Access Modifiers

- `public:` - Accessible from anywhere
- `private:` - Only accessible in the class

### Constructors and Destructors

**Constructors** initialize objects when created:
- Same name as the class
- No return type
- Can have parameters

**Destructors** clean up when objects are destroyed:
- Class name with `~` prefix
- No parameters
- No return type
- Called automatically when object goes out of scope

```pallas
class Resource {
    // Constructor
    Resource(filename: string) {
        println("Opening ${filename}");
        // Initialize resource
    }
    
    // Destructor
    ~Resource() {
        println("Closing resource");
        // Clean up
    }
}
```

### Object Instantiation

```pallas
// Stack allocation
person: Person = Person("Alice", 30);

// Heap allocation
ptr: Person* = new Person("Bob", 25);

// Method calls
person.greet();
ptr->greet();

// Cleanup
delete ptr;
```

---


## Memory Management

### Manual Control

Like C, you are responsible for memory management:
- No garbage collection
- Explicit allocation/deallocation

### Allocation and Deallocation

```pallas
// Allocate single object
ptr: i32* = new i32;
obj: Person* = new Person("Alice", 30);

// Allocate arrays
array: i32* = new i32[10];

// Use the memory
*ptr = 42;
array[0] = 100;

// Free memory when done
delete ptr;
delete obj;
delete array;
```

### Stack vs Heap

```pallas
// Stack allocation (automatic cleanup)
x: i32 = 5;
person: Person = Person("Bob", 25);

// Heap allocation (manual cleanup required)
ptr: i32* = new i32;
obj: Person* = new Person("Alice", 30);

// Manually delete heap allocated variables
delete ptr;
delete obj;
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
    println("${i}");
}

// Variations
for (; condition; ) {
    // While-like loop
}

for (;;) {
    // Infinite loop
    break;
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



#### Pattern Types

**Literal Patterns**
```pallas
match (x) {
    0 => { println("zero"); }
    1 => { println("one"); }
    42 => { println("the answer"); }
    _ => { println("something else"); }
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
    println("Hello, ${name}");
}

// Implicit void (no return type specified)
print_header() {
    println("=== Header ===");
}

// No parameters
get_random(): i32 {
    return 42;
}
```

### Variadic Functions

Functions that accept a variable number of arguments:

```pallas
sum(first: i32, ...): i32 {
    // Implementation accesses variadic arguments
    result: i32 = first;
    // Exact mechanism TBD
    return result;
}

// Usage
sum(1, 2, 3, 4, 5);
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

## String Interpolation

Strings support embedded expressions using `${expression}` syntax:

```pallas
name: string = "Alice";
age: i32 = 25;

// Basic interpolation
greeting: string = "Hello, ${name}!";

// Multiple expressions
info: string = "${name} is ${age} years old.";

// Expressions in interpolation
next_year: string = "Next year, ${name} will be ${age + 1} years old.";

// Complex expressions
calculation: string = "2 + 2 = ${2 + 2}";
double_age: i32 = age * 2;
complex: string = "Double age: ${double_age}, half: ${age / 2}";
```

---

## Module System

### Import Statements

```pallas
// Import entire module
import std/io;
import std/collections;

// Relative imports
import ./utils/math;
import ../common/helpers;
```

### Module Path Syntax

- `/` separates module components
- `./` for relative import from current directory
- `../` for relative import from parent directory

```pallas
import std/io;              // Standard library
import ./local/module;      // Local relative path
import ../parent/sibling;   // Parent directory
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
++  // Increment (prefix and postfix)
--  // Decrement (prefix and postfix)
```

### Assignment Operators

```pallas
=   // Assignment
+=  // Add and assign
-=  // Subtract and assign
*=  // Multiply and assign
/=  // Divide and assign
%=  // Modulo and assign
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
?:  // Ternary conditional operator
[]  // Array subscript
()  // Function call
&   // Address-of (prefix)
*   // Dereference (prefix)
```

---


### String Interpolation Example

```pallas
calculate_stats(numbers: i32*, count: i32): void {
    sum: i32 = 0;
    i: i32 = 0;
    
    while (i < count) {
        sum += numbers[i];
        i++;
    }
    
    avg: f32 = (f32)sum / (f32)count;
    
    println("Statistics:");
    println("  Count: ${count}");
    println("  Sum: ${sum}");
    println("  Average: ${avg}");
    println("  First: ${numbers[0]}, Last: ${numbers[count - 1]}");
}
```

---

## Design Decisions

### Under Consideration
- Range-based for loops
- Lambda syntax
- Generics/templates syntax
- Inheritance support
- Operator overloading
