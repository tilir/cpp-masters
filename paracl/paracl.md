# ParaCL Specification (Draft)

## 1.0 Introduction

**ParaCL** is a statically typed programming language with extensive type deduction and lightweight syntax.
It is designed primarily for educational purposes and focuses on explicit data flow, structural composition, and strong typing with predictable semantics.

---

## 1.1 Keywords and Program Structure

The following keywords are reserved and cannot be used as identifiers:

```
input, output, repeat, glue, bind, if, else, for, in, return, char, int, float, double
```

Every program consists of one or more **translation units (TUs)**.
A TU is a sequence of statements that must be executed in the specified order.

Example:

```pcl
// square.pcl
v0 : int = input(0);
output(0, v0);
```

As shown above, comments start with `//` and continue to the end of the line.

---

## 1.2 Types

Every entity (variable, constant, function, etc.) is introduced upon its first mention.
An entity always has an associated type, which can be either explicitly specified or automatically deduced by the compiler.

The special entities `input` and `output` are generic I/O primitives.
Conceptually, `input(0)` behaves like a typed standard input stream, and `output(0, value)` behaves like a typed standard output stream.

Example:

```pcl
v0 = 0;
v1 : double = v0;
v2 = input(0) : int;   // not input(v0)
v3 : int;
output(0, v1);
```

For integer types, you can explicitly specify the bit width:

```pcl
v0 : int(16) = 0; // 16-bit integer
```

By default, `int` is a 32-bit signed integer.

Integers use **two’s complement** representation and arithmetic wraps around on overflow—there is no undefined behavior.

The `char` type is a raw byte type, similar to `std::byte` in C++.

---

## 1.3 Arrays

An **array** is a contiguous collection of elements of the same type.
Arrays are stack-allocated. If the size is compile-time known, the array has a fixed length; otherwise, it behaves like a variable-length array (VLA).

A **vector** is a SIMD container that can only hold primitive element types.
Vectors always have compile-time known sizes and indices.

The `repeat` expression is syntactic sugar for constructing an array with repeated elements:

```pcl
arr0 = repeat(v0, 5);              // array of 5 identical elements
arr1 : int[5] = {1, 2, 3, 4};      // fixed-size array
arr2 : vector<int, 5> = {1, 2, 3, 4, 5}; // SIMD vector

v3 = 3;
v4 = arr0[v3];
v5 = arr2[3];                      // vector indexing requires constant index
arr_inputs = input(0..3) : int[4]; // array of 4 inputs
```

---

## 1.4 Structures

A **structure** is a compound entity that combines multiple entities of possibly different types.
Structures can be created using the `glue` expression, which aggregates several entities into one.

Members can be accessed by compile-time-known indices or by explicitly assigned names.

Examples:

```pcl
s0 = glue(v0, v2);
v3 = s0[0];

s1 = glue(s0 : x, v0 : y, arr0 : z);
v5 = s1.y;           // access by name
v6 = s1.z[0];

s2 : { x : int[5], y : double } = glue(repeat(v0, 5), v1);
s2.x = repeat(0, 5);
```

In the last example, a structure with an array field `x` and a scalar field `y` is created.

---

## 1.5 Functions

A **function** is an abstraction that encapsulates a sequence of expressions.
The result of a function is the value of its last expression, unless an explicit `return` is used.

Inside a function, the `input` expression returns the value of the corresponding argument.
If a function argument’s type is not explicitly declared, the function is **generic**.

Examples:

```pcl
f0 = { v3 = input(0) + 1; v3 * v3; };
v7 = f0(2); // but not f0(2, 3)

f1 : (x) = { v3 = x + 1; v3 * v3; };  // x is generic
f2 : (x, y) = { v3 = x + y; v3 * v3; }; // both generic

f3 : (x : int, y : double) : int = { v3 = x + y; v3 * v3; };
v8 = f3(v0, v1);
```

---

## 1.6 Methods

Functions can be **bound** to structures, turning them into **methods**.
The `bind` expression associates a function with a structure, optionally pre-filling some of its parameters with structure fields.

If the number of bound members is smaller than the number of parameters, the remaining ones must be passed explicitly when calling the method.

Examples:

```pcl
s3 : { x : int, g : () } = glue(v0, bind(f1, s3.x)); // bind requires function name
v9 = s3.g(); // (v0 + 1) * (v0 + 1)

s4 : { x : int, g : () } = glue(v0, bind(f2, s4.x));
v9 = s4.g(v1); // equivalent to f2(s4.x, v1)
```

---

## 1.7 Conditionals

Conditional statements use familiar syntax.
Both single-statement and block forms are supported:

```pcl
if (v2 == 0)
  v0 = f1(v0);

if (v2 == 0) {
  v0 = f1(v0);
}

if (v0 == v2) {
  v0 = f1(v0);
}
else {
  v2 = f1(v2);
}
```

---

## 1.8 Function Returns and Abbreviations

Functions can return early using the `return` statement.

Example:

```pcl
f4 : (x) = { if (x < 2) return 1; x * f4(x - 1); };
```

If a function body contains only a single expression, braces can be omitted:

```pcl
f5 : (x) = x * x;
f6 : (x) = { x * x; };
f7 : (x) = { return x * x; };
```

Functions with **generic** parameter types cannot be assigned to each other:

```pcl
f5 = f4; // invalid — types are generic
```

However, functions with **concrete** (fully specified) types can be reassigned:

```pcl
f8 : (x : int) = x + 1;
f9 : (x : int) = x * x + 1;
f8 = f9; // valid
```

---

## 1.9 Loops

Loop syntax is conventional and expressive.
Iteration over arrays is allowed, but **vectors cannot be iterated**.

Example:

```pcl
v0 = 5;
for (x in 0:5)          // or (x : int), (0:5:1), etc.
  v0 = v0 + x;

for (x in arr0) {        // array iteration
  v0 = v0 + x;
}
```

Additionally, ParaCL provides a `while` loop that executes while a condition remains true (non-zero).

Example: Fibonacci sequence

```pcl
n = 0;
a = 0;
b = 1;
x : int = input(0);

while (n < x) {
  n = n + 1;

  if (n == 1)
    output(0, a);

  if (n == 2)
    output(0, b);

  if (n > 2) {
    tmp = b;
    b = a + b;
    a = tmp;
    output(0, b);
  }
}
```

