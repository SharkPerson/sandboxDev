package main

import (
    "fmt"
    "net/http"
    "html/template"
)

type Todo struct {
    Title string
    Done bool
}

type TodoPageData struct {
    PageTitle string
    Todos []Todo
}

func doTemplate() {
    tmpl := template.Must(template.ParseFiles("todoListLayout.html"))
    http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
        data := TodoPageData{
            PageTitle: "My TODO list",
            Todos: []Todo{
                {Title: "Task 1", Done: false},
                {Title: "Task 2", Done: true},
                {Title: "Task 3", Done: false},
            },
        }
    tmpl.Execute(w, data)
    })
}


func printCurrentPage(w http.ResponseWriter, r *http.Request) {
    fmt.Fprintf(w, "You have requested the %s page", r.URL.Path)
}

func main() {
    http.HandleFunc("/", printCurrentPage)
    doTemplate() 
    http.ListenAndServe(":8080", nil)
}
