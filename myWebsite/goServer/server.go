package main

import (
    "fmt"
    "os"
    "log"
    "net/http"
    "html/template"
    "regexp"
    "errors"
    "sync"
)

type Page struct {
    Title string
    Body []byte // byte slice
}

var counter int
var mutex = &sync.Mutex{}

// Template caching
// Parse Files during initialisation so that all templates are in a single pointer to Template.
var templates = template.Must(template.ParseFiles("edit.html", "view.html"))
// Must compile will parse and compile the regex and return a regexp. MustCompile will panic if the compilation fails.
var validPath= regexp.MustCompile("^/(edit|save|view)/([a-zA-Z0-9]+)$")

func incrementCounter(w http.ResponseWriter, r* http.Request) {
    mutex.Lock();
    counter++
    fmt.Fprintf(w, "%d", counter)
    mutex.Unlock()
}

//    param    name    return 
// Takes a receiver (pointer to Page), returns type of error (os.WriteFile returns an error, nil - if nothing, else, error code)
func (p *Page) save() error {
    filename := p.Title + ".txt"
    return os.WriteFile(filename, p.Body, 0600)
}

// Constructs a filename from the input string, reads the body from the file and returns a pointer to the Page.
func loadPage(title string) (*Page, error) {
    filename := title + ".txt"
    body, err := os.ReadFile(filename)
    if err != nil {
        return nil, err
    }
    return &Page{Title: title, Body: body}, nil
}

func renderTemplate(w http.ResponseWriter, tmpl string, p *Page) {
    // Reads the contents of edit, returns a pointer to template.Template
    //t, err := template.ParseFiles(tmpl + ".html")
    //if err != nil {
    //    http.Error(w, err.Error(), http.StatusInternalServerError)
    //    return
    //}
    //// Executes the template, writes generated HTML to w, the http.ResponseWriter
    //err = t.Execute(w, p)
    //if err != nil {
    //    http.Error(w, err.Error(), http.StatusInternalServerError)
    //}
    err := templates.ExecuteTemplate(w, tmpl + ".html", p)
    if err != nil {
        http.Error(w, err.Error(), http.StatusInternalServerError)
    }
}

func makeHandler(fn func (http.ResponseWriter, *http.Request, string)) http.HandlerFunc {
    return func(w http.ResponseWriter, r *http.Request) {
        m := validPath.FindStringSubmatch(r.URL.Path)
        if m == nil {
            http.NotFound(w, r)
            return
        }
        fn(w, r, m[2])
    }
}

func viewHandler(w http.ResponseWriter, r *http.Request, title string) {
    //title := r.URL.Path[len("/view/"):]
    //title, err := getTitle(w, r)
    p, err := loadPage(title)
    // if the page doesn't exist, redirect them to the edit page
    if err != nil {
        http.Redirect(w, r, "/edit/" + title, http.StatusFound)
        return
    }
    renderTemplate(w, "view", p)
}

func editHandler(w http.ResponseWriter, r *http.Request, title string) {
    //title := r.URL.Path[len("/edit/"):]
    //title, err := getTitle(w, r)
    p, err := loadPage(title)
    if err != nil {
        p = &Page{Title: title}
    }
    renderTemplate(w, "edit", p)

    //fmt.Fprintf(w, "<h1>Editing %s</h1>" +
    //    "<form action=\"/save/%s\" method\"POST\"<"+
    //    "textarea name=\"body\">%s</textarea><br>"+
    //    "<input type\"submit\" value=\"Save\">"+
    //    "</form>",
    //    p.Title, p.Title, p.Body)
}

func saveHandler(w http.ResponseWriter, r *http.Request, title string){
    //title := r.URL.Path[len("/save/"):]
    //title, err := getTitle(w, r)
    body := r.FormValue("body")
    // Creates a page with the title from the save path, and the body text value
    p := &Page{Title: title, Body: []byte(body)}
    err := p.save()
    if err != nil {
        http.Error(w, err.Error(), http.StatusInternalServerError)
        return
    }
    // Redirects back to the view page
    http.Redirect(w, r, "/view/" + title, http.StatusFound)
}

func getTitle(w http.ResponseWriter, r *http.Request) (string, error) {
    m := validPath.FindStringSubmatch(r.URL.Path)
    // If title is invalid, writes a 404 Not Found error to the HTTP connection.
    if m == nil {
        http.NotFound(w, r)
        return "", errors.New("invalid Page Title")
    }
    // If the title is valid, returns the title along with a nil error
    return m[2], nil
}


//func main() {
//    //p1 := &Page{Title: "TestPage", Body: []byte("This is a sample Page.")}
//    //p1.save();
//    //p2, _ := loadPage("TestPage")
//    //fmt.Println(string(p2.Body))
//    
//    fmt.Println("Running server on port :8080")
//    http.HandleFunc("/", func(w http.ResponseWriter, r* http.Request) { 
//        http.ServeFile(w, r, r.URL.Path[:1])
//    })
//    http.HandleFunc("/view/", makeHandler(viewHandler))
//    http.HandleFunc("/edit/", makeHandler(editHandler))
//    http.HandleFunc("/save/", makeHandler(saveHandler))
//    http.HandleFunc("/", incrementCounter)
//    log.Fatal(http.ListenAndServe(":8080", nil))
//}

