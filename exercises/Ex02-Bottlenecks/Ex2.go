// Go 1.2
// go run helloworld_go.go

package main

import (
    . "fmt"
    "runtime"
    "sync"
    "time"
)

var i int
var mutex_i = &sync.Mutex{}

func increase_i() {
		for j := 0; j < 1000000; j++ {
				mutex_i.Lock()
				i++
				mutex_i.Unlock()
		}
}

func decrease_i() {
		for j := 0; j < 1000000; j++ {
				mutex_i.Lock()
				i--
				mutex_i.Unlock()
		}
}

func main() {
    runtime.GOMAXPROCS(runtime.NumCPU())    // I guess this is a hint to what GOMAXPROCS does...
                                            // Try doing the exercise both with and without it!
    go increase_i()                      // This spawns someGoroutine() as a goroutine
    go decrease_i()                      // This spawns someGoroutine() as a goroutine

    // We have no way to wait for the completion of a goroutine (without additional syncronization of some sort)
    // We'll come back to using channels in Exercise 2. For now: Sleep.
    time.Sleep(1000*time.Millisecond) // Lazy joining of threads #SWAG
    Println(i)
}
