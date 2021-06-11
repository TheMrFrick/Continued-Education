# Golang Notes

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

## Functions
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

