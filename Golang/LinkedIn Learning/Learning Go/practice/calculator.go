package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	reader := bufio.NewReader(os.Stdin)
	fmt.Print("Value 1: ")
	val1, _ := reader.ReadString('\n')
	val1F, err := strconv.ParseFloat(strings.TrimSpace(val1), 64)
	if err != nil {
		panic(err)
	}

	fmt.Print("Value 2: ")
	val2, _ := reader.ReadString('\n')
	val2F, err := strconv.ParseFloat(strings.TrimSpace(val2), 64)
	if err != nil {
		panic(err)
	}

	fmt.Printf("The sum of %v and %v is %v", val1F, val2F, (val1F + val2F))

}
