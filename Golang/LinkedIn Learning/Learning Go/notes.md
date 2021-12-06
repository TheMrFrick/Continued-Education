# Explore Go's Variable Types

## Boolean and String Types
- Boolean
- - true/false
- String
- - string - collection of characters

## Integer Types
- Fixed Integer types
- - uint8
- - uint16
- - uint32
- - uint64
- - int8

- Aliases can be used instead of full type names
- - byte
- - uint
- - int 
- - uintptr

## Other Numeric Types
- Floating values
- - float32
- - float64
- Complex Numbers
- - complex64
- - complex128

## Complex Types
- Data collection
- - Arrays
- - Slices
- - Maps
- - Structs
- Language Organization
- - Functions
- - Interfaces
- - Channels
- Data Management
- - Pointers

# Using Math Operators
## Math Operators
- Go supports all math operators used in C
- - '+' = sum
- - '-' = difference
- - '*' = product
- - '/' = quotient
- - '%' = remainder/modulo
- - '&' = bitwise AND
- - '|' = bitwise OR
- - '^' = bitwise XOR
- - '&^' = bit clear
- - '<<' = left shift
- - '>>' = right shift

## Math requires identical types
- Numeric types don't implicitly convert

## Conver Types before Using
- wrap value in target type as a function call
```Go
var anInt int = 5
var aFloat float64 = 42
sum := float64(anInt) + aFloat
fmt.Printf("Sum: %v, Type: %T\n",sum, sum)
```

## The Math Package
- For other operations, use math package
```Go
import "math"

var aFloat = 3.14159
var rounded = math.Round(aFloat)
fmt.Printf("Original: %v, Rounded: %v\n",aFloat, rounded)
```
# How memory is allocated and managed
## Memory is Managed by the Runtime
- The Go runtime is statically linked into application
- Memory is allocated and deallocated automatically
- Use `make()` or `new()` to initialize maps, slices, and channels

## Memory Allocation
The `new()` function
- Allocated but does not initialize memory
- Results in zeroed storage but returns a memory address

The `make()` function
- Allocates and initializes memory
- Allocates non-zeroed storage and returns a memory address

