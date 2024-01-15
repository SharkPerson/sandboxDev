package main

import (
    "testing"
)

func TestHello(t *testing.T) {
    result := Hello()
    want := "Hello world"

    if result != want {
        t.Errorf("got %q, wanted %q", result, want)
    }
}
