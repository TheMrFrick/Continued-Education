# Golang Notes

## First Step in Go
Golang is built as an inbetween or a mixture between statically-typed languages and dynamically-typed interpreted languages.
"Go is an attempt to combine the safety and performance of a statically-typed language with the expressiveness and convenience of a dynamiccaly-typed interpreted language." - Rob Pike

Go borrows ideas from different languages that came before it, including:
- Simplified but concise syntax that is fun and easy to use
- A type of system that feels more like a dynamic language
- Support for object-oriented programming
- Statically typed for compilation and runtime safety
- Compiled to native binaries for fast runtime execution
- Near-zero compilation time that feels more like an interpreted language
- A simple concurrency idiom to leverage multi-core, multi-chip machines
- A garbage collector for safe and automatic memory management

### Functions
Go programs are composed of functions, the smallest callable code unit in the language. 
Functions are typed entities that can either be named or be assigned to a variable as a value

```Go
// a simple Go function
func moles(mass amu) float64 {
    return float64(mass) / grams
}
```

Go functions can return multiple values as a result of a call
```Go
func moles(mass amu) (float64, error) {
    if mass < 0 {
        return 0, error.New("invalid mass")
    }
    return (float64(mass) / grams), nil
}
```

### An Extensive Library
The following, by no means exhaustive, lists some of the core APIs that programmers get out-of-the-box:
- Complete support for regular expressions with search and replace
- Powerful IO primitives for reading and writing bytes
- Full support for networking from socket, TCP/UDP, IPv4, and IPv6
- APIs for writing production-ready HTTP services and clients
- Support for traditional synchronization primitives (mutex, atomic, and so on)
- General-purpose template framework with HTML support
- Support for JSON/XML serializations
- RPC with multiple wire formats
- APIS for archive and compression algorithms: tar, zip/gzip, zlib, and so on
- Cryptography support for most major algorithms and hash functions
- Access to OS-level processes, envrionment info, signaling, and much more.

### Go Toolchain
Golang tools and a summary of what they do.
- `fmt`: Reformats source code to adhere to the standard
- `vet`: Reports improper usage of source code constructs
- `lint`: Another source code tool that reports flagrant style infractions
- `goimports`: Analyzes and fixes package import references in source code
- `godoc`: Generates and organizes source code documentation
- `generate`: Generates Go source code from directives store in source code
- `get`: Remotely retrieves and installs packages and their dependencies
- `build`: Compiles code in a specified package and their dependencies
- `run`: Provides the convenience of compiling and running your Go program
- `test`: Performs unit tests with support for benchmark and coverage reports
- `oracle` static anaylsis tool: Queries source code structures and elements
- `cgo`: Generates source code for interoperability between Go and C

## Go Languages Essentials

### Go Source File
Typical Go source files can be split up into three main sections:
1. __Package Clause__
```Go
//1 package Clause
package main
```
2. __Import Declaration__
```Go
//2 Import Declaration
import "fmt"
import "math/rand"
import "time"
```
3. __Source Body__
```Go
//3 Source Body
var greeting = [][]string{
    {"Hello, World!","English"},
    ...
}

func greeting() [] string{
    ...
}

func main() {
    ...
}
```
Semicolons are optional in Golang. They are not idiomatic Go, but they also don't hurt to explicitely state the termination of a statement.

Multiple line breaks:

```Go
lonStr := "Hello World!" +
"How are you?"
```

> The Go toolchain includes the gofmt tool, which can be used to consistently apply proper formatting rules to your source code.
> There is also the `govet` tool, which goes much further by analyzing your code for structural problems with code elements.

### Types

| Category | Identifier |
| -------- | ---------- |
| Numeric | byte, int, int8, int16, int32, int64, rune, uint, uint8, uint16, uint32, uint64, float32, float64, complex64, uintptr |
| String | string |
| Boolean | bool | 
| Error | error |

### Values

| Category | Identifier |
| -------- | ---------- |
| Boolean constants | true,false |
| Constant counter | iota |
| Uninitialized value | nil |

### Functions
The following functions are available as part of Go's built-in pre-declared identifiers:

| Category | Identifier |
| -------- | ---------- |
| Initialization | make(),new() |
| Collections | append(),cap(),copy(),delete() |
| Complex numbers | complex(),imag(),real() |
| Error Handling | panic(),recover() |

### Declaring a variable
```Go
var <name of variable> <type> = <value> //long hand
<name of variable> := <value> //short-hand
```

**Restrictions for short variable declaration**
- Firstly, it can only be used within a function block
- The assignment operator :=, declares variable and assign values
- := cannot be used to update a previously declared variable
- Updates to variables must be done with an equal sign

**Variable scope and visibility**
General rule, a variable is only accessible from within the block where it is declared and visible to all nested sub-blocks.