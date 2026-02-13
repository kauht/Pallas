# Language Design

---

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
- **Signed integers**: `i8`, `i16`, `i32`, `i64`, `i128`
- **Unsigned integers**: `u8`, `u16`, `u32`, `u64`, `u128`
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
i32**      // Pointer to pointer to i32
i32[10]    // Array of 10 i32s
i32*[5]    // Array of 5 pointers to i32
```

---

## Syntax

### Control Flow

```pallas

// ifs
if (x == 2 || x == 1) {
    // code
} else if (x == 3) {
    // code
} else {
    println("x is more than 3 or less than 1");
}


while (true == true) {
    // code
}

do {
    // code
} while(true == false);

// Iterator For
for (i: i32 = 0; i < 10; ++i) {
    println("Yo {}!", i);
}

// For-Each
for (i: i32 : object) {
    println("Yo {}!", i);
}

// Match Expressions (more powerful alternative to C style switch statements)
x: i32 = 1;

match (x) {
    1 => {println("One!");}
    2 || 3 => {println("Two or Three!");}
    4 => {println("Four!");}
    99 => {}
    _ => {println("Else...");}
}

```

---

## Functions

```
// Basic
add(x: i32, y: i32): i32 {
    return x + y;
}

// No Return
do_nothing(): void {
    // Nothing...
}
do_something() { // Implicit void
    println("Something");
}

// Variadics

sum(first: i32, ...): i32 {
    sum: i32 = 0;
    for (auto i : {...}) {
        if (sizeof(i) != sizeof(i32)) {
            return NULL;
        }
        sum+=(i32)i;
    }
    return sum;
}

// Generics

make_ptr<T>(x: T): T* {
    ret: T* = new T;
    *ret = x;
    return ret;
}

```

---

## Objects
```pallas
// Class
class Person {
    public {
        name: string;
        age: i8;
        
        // Constructor
        Person(n: string, a: i8) {
            name = n;
            age = a;
        }
        ~Person() {
        }
        
        greet(): void {
            println("Hello, {}! You are {} years old.", name, age);
        }
    }
    private {
        ssn: string;
        validate(): bool {
            return age >= 18;
        }
    }

}

// Struct
struct Vector3 {
    x: f64;
    y: f64;
    z: f64;
}

```

---

## Memory Management

```pallas
// Stack Alloc
sammy: Person = Person("Sammy", "17");

// Heap Alloc
sammyptr: Person* = new Person("Sammy", "17");

// Method Calls
sammy.greet();
sammyptr->greet();

delete sammyptr;
```

---

## Extras

```pallas
// Constants
const PI: f64 = 3.141592653589793238;

// String Interpolation
name: string = "Alice";
greeting: string = "Hello, ${name}!";

calculation: string = "2 + 2 = ${2 + 2}";

// Imports
import std/io;
import ../common/common; // Import common.adan from common directory



// Arithmetic Operators
+   // Addition
-   // Subtraction
*   // Multiplication
/   // Division
%   // Modulo
++  // Increment (prefix and postfix)
--  // Decrement (prefix and postfix)

// Assignment Operators
=   // Assignment
+=  // Add and assign
-=  // Subtract and assign
*=  // Multiply and assign
/=  // Divide and assign
%=  // Modulo and assign

// Comparison Operators
==  // Equal to
!=  // Not equal to
<   // Less than
<=  // Less than or equal to
>   // Greater than
>=  // Greater than or equal to

// Logical Operators
&&  // Logical AND
||  // Logical OR
!   // Logical NOT

// Bitwise Operators
&   // Bitwise AND
|   // Bitwise OR
^   // Bitwise XOR
~   // Bitwise NOT
<<  // Left shift
>>  // Right shift

// Other Operators
?:  // Ternary conditional operator
[]  // Array subscript
()  // Function call
&   // Address-of (prefix)
*   // Dereference (prefix)

// Lambdas

```

---


### Still Considering
- Range-based for loops?
- Generics/Template syntax
- Inheritance?
- Operator Overloading
