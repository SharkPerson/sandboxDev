package token

type TokenType string

type Token struct {
    Type TokenType  // a type to differentiate the type of the token
    Literal string  // the literal value of the token. using a string is verstatile but won't be as performant as using int or char.
}

// Define the token type constants
const (
    ILLEGAL = "ILLEGAL"
    EOF = "EOF"

    // Identifiers and literals
    IDENT = "IDENT"
    INT = "INT"

    // Operators
    ASSIGN = "="
    PLUS = "+"
    MINUS = "-"
    BANG = "!"
    ASTERISK = "*"
    SLASH = "/"

    LT = "<"
    GT = ">"

    // Delimiters
    COMMA = ","
    SEMICOLON = ";"

    LPAREN = "("
    RPAREN = ")"
    LBRACE = "{"
    RBRACE = "}"

    // Keywords
    FUNCTION = "FUNCTION"
    LET = "LET"
    TRUE = "TRUE"
    FALSE = "FALSE"
    IF = "IF"
    ELSE = "ELSE"
    RETURN = "RETURN"
)

var keywords = map[string]TokenType{
    "fn": FUNCTION,
    "let" : LET,
    "true" : TRUE,
    "false" : FALSE,
    "if" : IF,
    "else" : ELSE,
    "return" : RETURN,
}

func LookupIdent(input string) TokenType {
    if tok, ok := keywords[input]; ok { 
        return tok 
    }
    return IDENT 
}
