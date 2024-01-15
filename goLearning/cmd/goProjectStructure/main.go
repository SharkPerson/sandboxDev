package main

import (
	"fmt"
	"log"
	"runtime"
	"strconv"
	"strings"
	"time"
)

// consts get evaluated at compile time
const RANDOM_NUM = 1 // do not need type
const PI float32 = 3.19

type Gender int 
const (
    UNKNOWN = iota
    FEMALE
    MALE
)


func printingStuff() {
    value := 1
    fmt.Printf("%d\n", value)
    floatVal := 3.2
    fmt.Printf("%g\n", floatVal)
    digitVal := 12345
    fmt.Printf("%01d\n", digitVal) // shows integer with n digits
    // Complex numbers
    var c1 complex64 = 5 + 10i
    fmt.Printf("Complex value: %v\n", c1)
    // Character types
    var ch byte = 'A'
    var ch1 byte = 65
    var ch2 byte = '\x41'
    fmt.Printf("Same right? %c %c %c\n", ch, ch1, ch2)
}

func stringsStuff() {
    // Go strings can vary from 1 - 4 bytes, smart enough to know which to use
    // Strings are immutable array of bytes
    interpretedString := "this is an interpreted string using \"\" double quotes"
    rawString := `this is a raw string \n is taken literally`
    fmt.Printf("%s\n", interpretedString)
    fmt.Printf("%s\n", rawString)
    // Strings in Go do not terminate with a special character \0 like in C and C++
    
    // Prefix and suff
    var str string = "This is an nice example of an example string"
    fmt.Printf("Is there prefix: %t\n", strings.HasPrefix(str, "Th"))
    fmt.Printf("Is there suffix: %t\n", strings.HasSuffix(str, "ing"))

    // Can also test for substring
    str1 := "example"
    fmt.Printf("Is \"%s\" a substring of \"%s\": %t\n", str1, str, strings.Contains(str, str1)) 
    var index int = strings.Index(str, str1)
    var lastIndex int = strings.LastIndex(str, str1)
    var substringCount int = strings.Count(str, str1)
    // For non ASCII chars, use strings.IndexRune()
    fmt.Printf("Substring index: %d, last index: %d, count: %d\n", index, lastIndex, substringCount)

    // Replacing Substring
    strReplaced := strings.Replace(str, "example", "hello", 1)    
    fmt.Println(strReplaced)
    // Repeating a string
    repeatedString := strings.Repeat(str, 2)
    fmt.Println(repeatedString)

    // Changing case of the string 
    upperCaseString := strings.ToLower(str)
    fmt.Println(upperCaseString)
    lowerCaseString := strings.ToUpper(str)
    fmt.Println(lowerCaseString)
    // Removes all leading and trailing whitespace 
    noSpaceString := strings.TrimSpace(str)
    fmt.Println(noSpaceString)

    // Splitting a string
    fieldString := strings.Fields(str)
    fmt.Println(fieldString)

    // Reading from a string 
    floatString := "32.45"
    newFloat, err := strconv.ParseFloat(floatString, len(floatString))
    if err != nil {
        fmt.Printf("Got an error\n")
    }
    fmt.Printf("%g\n", newFloat)
}

func timeStuff() {
    t := time.Now()
    fmt.Printf("The date is: %02d %02d %04d\n", t.Day(), t.Month(), t.Year())
    
}

func controlStructures() {
    for i := 0; i < 5; i++ {
        fmt.Println("For loop")
    }

    // While loop also uses for 
    i := 0
    for i < 3{
        fmt.Println("While loop")
        i++
    }

    // for range
    // The range keyword is applied to an indexed collection coll
    str := "Go is a beautiful language"
    for pos, char := range str {
        fmt.Printf("Character on position %d is: %c \n", pos, char)
    }
}

// Declare a function as a type
type binOp func(int, int) int

func functionStuff() {
    // No function overloading allowed
    // A function that returns 3 arguments can be a param for a function that takes 3 arguments
    
    // Functions can be declared as a function type
    // Can be assigned to a variable
    //add := binOp
    // add gets a reference to the function, knows the signature, when not assigned, it is nil

    // defer - LIFO in the function scope
    // Good for - closing file streams, unlocked a mutex, printing a footer, closing a database connection
    defer fmt.Println("this has been deferred")
    // Can be used in debugging by tracing in and out of functionStuff
    // Can be used for logging output at the end as well
    
    // Common built in Functions
    // close(), len(), cap() capacity
    // new(), make(), allocating memory. new() is for value types, make() is for built in reference types like maps, channels, slices.

    // copy(), append() for slices
    
    // panic(), recover() for error handling

    // print and println, low-level, use fmt ones

    // complex, real, imag, for complex numbers

    // Higher order functions 
    // Functions can be used as a parameter in another function. The passed function can be called within the body of that function (callback)
    // Filter functions


    // Closures/Function literal/Lambda function
    // Anonymous function, do not want to give the function a name
    // These can be used with the defer to change return values if using named return parameters. Also, can be launched as goroutines.
    fplus := func(x, y int) int {return x + y}
    fmt.Printf("%d\n", fplus(3, 4))

    // Debugging with closures
    where := func() {
        _, file, line, _ := runtime.Caller(1)
        log.Printf("%s:%d", file, line)
    }
    fmt.Println("Using the runtime.Caller function")
    where()

    // Can achieve the same using log
    log.SetFlags(log.Llongfile)
    var where2 = log.Print
    fmt.Println("Using the log.Print function")
    where2()

    // Timing a function 
    calculationFunc := func() {
        for i := 0; i < 100000000; i++ {
            // do something
        }
    }

    startTime := time.Now()
    calculationFunc()
    endTime := time.Now()
    deltaTime := endTime.Sub(startTime)
    fmt.Printf("Calculation time: %s\n", &deltaTime)
}

// Solving the fibonacci sequence
func memoization() {
    result := 1
    a := 1
    b := 1

    startTime := time.Now()
    for i := 0; i <= 10; i++ {
        if i > 1 {
            result = a + b
        }
        fmt.Printf("fibonacci(%d) is: %d\n", i, result)

        // modifying two values for next iteration
        a = b
        b = result
    }
    endTime := time.Now()
    deltaTime := endTime.Sub(startTime)
    fmt.Printf("Fibonacci calculation time: %s\n", &deltaTime)
}

// Returning a function using closures
// genInc returns a function that returns an int
func genInc(n int) func (x int) int {
    return func(x int) int {
        return x + n
    }
}

// Factory function
func MakeAddSuffix(suffix string) func(string) string {
    return func(name string) string {
        if !strings.HasSuffix(name, suffix) {
            return name + suffix
        }
        return name 
    }
}

func arraysAndSlices() {
    // Arrays are inflexible, slices are usually used instead (like python lists)
    // Here is an arraysAndSlices
    var arr [5]int
    for i := 0; i < len(arr); i++ {
        arr[i] = i * 2
        fmt.Printf("value is: %d\n", arr[i])
    }
    var arr2 [3]int
    for i := range arr2 {
        arr2[i] = i * 2
        fmt.Printf("value is: %d\n", arr2[i])
    }

    // Assigning some arrays
    arr3  := arr2 // This copies the array
    for i := range arr3 {
        fmt.Printf("assigned to arr3: %d\n", arr3[i])
    }
    arr4 := &arr3
    for i := range arr3 {
        fmt.Printf("assigned to arr4: %d\n", arr4[i])
    }
    // Rearrange this value
    arr4[0] = 111
    fmt.Printf("Changed val of arr4 pointing to arr3, arr3 val: %d\n", arr3[0])
}

func main() {
    printingStuff()
    stringsStuff()
    timeStuff()
    controlStructures()
    functionStuff()
    val := genInc(1)(2)
    fmt.Printf("Function return function val: %d\n", val)
    // Creating more functions from the factory function
    addBmp := MakeAddSuffix(".bmp")
    addJpeg := MakeAddSuffix(".jpeg")
    fmt.Printf("bmp factory function: %s, jpeg factory function: %s\n", addBmp("file1"), addJpeg("file2"))
    memoization()
    arraysAndSlices()
}

