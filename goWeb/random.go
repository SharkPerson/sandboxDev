package main

import (
)

func displayMessage(msg string) func() string {
    return func() string {
        msg = "Hello from " + msg
        return msg
    }
}
