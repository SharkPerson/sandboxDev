package main

import (
    "fmt"
    "os"
)

type Page struct  {
    Title string
    Body []byte
}

func (p *Page) save() error {
    filename := p.Title + ".txt"
    return os.WriteFile(filename, p.Body, 0600)
}

func loadPage(title string) (*Page, error) {
    filename := title + ".txt"
    body, err := os.ReadFile(filename) 
    if err != nil {
        fmt.Printf("Error loading file: %s\n", body)
    }
    p := Page{Title: title, Body: body}
    return &p, err 
}

