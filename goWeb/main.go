package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net/http"
)

type Todo struct {
    UserId int `json:"userId"`
    Id int `json:"id"`
    Title string `json:"title"`
    Completed bool `json:"completed"`
}

type HandleFun func(http.ResponseWriter, *http.Request)

func main() {
    doPageStuff()
}

func doPageStuff() {
    p := Page{Title: "TestPage", Body: []byte("This is a test")}
    p.save()
    p2, _ := loadPage("TestPage")
    fmt.Println(string(p2.Body))
}

func doWebServerStuff() {
    var msg string = "ME!"
    fmt.Println(displayMessage(msg)())
    serveFiles()
    createServer()
}

func logPanics(function HandleFun) HandleFun {
    return func(w http.ResponseWriter, r* http.Request) {
        defer func() {
            if x := recover(); x != nil {
                log.Printf("[%v] caught panic: %v", r.RemoteAddr, x)
            }
        }()
        function(w, r)
    }
}

func logging(f http.HandlerFunc) http.HandlerFunc {
    return func(w http.ResponseWriter, r *http.Request) {
        defer func() {
            log.Println(r.URL.Path)
        }()
        f(w,r)
    }
}

func handleRoutes(w http.ResponseWriter, r* http.Request) {
    if len(r.URL.Path) < 9 {
        http.Error(w, "404 Not found\nYou're not allowed here", http.StatusNotFound)
        return
    }

    if r.URL.Path[:9] != "/security" {
        http.Error(w, "404 Not found\nYou're not allowed here", http.StatusNotFound)
        return
    }
    fmt.Printf("Hello, you visited %s\n", r.URL.RawPath)

    if r.Method != "GET" {
        http.Error(w, "Method is not allowed", http.StatusNotFound)
        return
    }

    if r.URL.Path == "/security/postStuff" {
        postRequest()
    }

    if r.URL.Path == "/security/getStuff" {
        retrieveJsonData()
    }

    if r.URL.Path == "/security/putStuff" {
        putReqest()
    }
}

func serveFiles() {
    fs := http.FileServer(http.Dir("assets/"))
    http.Handle("/security/static/", http.StripPrefix("/security/static/", fs))
}

func createServer() {
<<<<<<< Updated upstream
    port := ":8080"
    http.HandleFunc("/", logPanics(handleRoutes))
=======
    port := ":80"
    http.HandleFunc("/", logging(handleRoutes))
>>>>>>> Stashed changes
    fmt.Printf("Creating a server at %s\n", port[1:])
    err := http.ListenAndServe(port, nil)
    if err != nil {
        panic("Failed to create a server\n")
    }
}

func retrieveJsonData() {
    fmt.Println("Getting JSON data")
    response, err := http.Get("https://jsonplaceholder.typicode.com/posts")
    if err != nil {
        log.Println("Error getting data from the JSON API")
        return
    }
    defer response.Body.Close()

    bodyBytes, _ := io.ReadAll(response.Body)
    bodyString := string(bodyBytes)
    fmt.Printf("API repsonse as a string\n %s", bodyString) 
    var todoStruct Todo
    json.Unmarshal(bodyBytes, &todoStruct) 
    fmt.Printf("API repsonse struct %+v\n", todoStruct) 
}


func postRequest() {
    fmt.Println("Performing HTTP POST")
    todo := Todo{1, 2, "New todo task", true}
    jsonReq, err := json.Marshal(todo)
    if err != nil {
        log.Println("Failed to marshal JSON data")
    }
    // Convert the json data into a slice of bytes using new buffer
    // Three args, URL, content type (application/json)
    resp, err := http.Post("https://jsonplaceholder.typicode.com/posts", "application/json; charset=utf-8", bytes.NewBuffer(jsonReq))
    if err != nil {
        log.Println("Failed to POST JSON data")
    }
    defer resp.Body.Close()

    bodyBytes, _ := io.ReadAll(resp.Body)
    bodyString := string(bodyBytes)
    fmt.Printf("API repsonse as a string\n %s", bodyString) 
    var todoStruct Todo
    json.Unmarshal(bodyBytes, &todoStruct) 
    fmt.Printf("API repsonse struct %+v\n", todoStruct) 
}

func putReqest() {
    fmt.Println("Performing HTTP PUT")
    todo := Todo{2, 3, "Updated task", false}
    jsonReq, err := json.Marshal(todo)
    if err != nil {
        log.Println("Failed to marshal JSON data")
    }
    req, err := http.NewRequest(http.MethodPut, "https://jsonplaceholder.typicode.com/posts/1", bytes.NewBuffer(jsonReq))
    req.Header.Set("Content-Type", "application/json; charset=utf-8")
    client := &http.Client{}
    resp, err := client.Do(req)
    if err != nil {
        log.Println("Failed to PUT JSON data")
    }
    defer resp.Body.Close()

    bodyBytes, _ := io.ReadAll(resp.Body)
    bodyString := string(bodyBytes)
    fmt.Printf("API response a string:\n%s", bodyString)
}
