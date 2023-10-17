package lexer

import (
	"monkey/token"
)

type Lexer struct {
    input string
    position int // current position of the input (current char)
    readPosition int // current reading position (after the current char)
    ch byte // current char being examined
}

func New(input string) *Lexer {
    l := &Lexer{input: input}
    l.readChar()
    return l
}

// This function reads the character until the end of the string.
// This function only supports ASCII characters.
// TODO Make this support unicode characters as well.
func (l *Lexer) readChar() {
    // Checks if the end is reached or not
    if l.readPosition >= len(l.input) {
        l.ch = 0 // ASCII code for NULL
    } else {
        l.ch = l.input[l.readPosition]
    }
    l.position = l.readPosition
    l.readPosition += 1
}

// Returns the token of the character being read. Calls the readChar function to get the next character.
// Assigns the token based on the character and then returns that token.
func (l *Lexer) NextToken() token.Token {
    var tok token.Token
    l.skipWhitespace()
    switch l.ch {
    case '=':
        tok = newToken(token.ASSIGN, l.ch)
    case '+':
        tok = newToken(token.PLUS, l.ch)
    case '(':
        tok = newToken(token.LPAREN, l.ch)
    case ')':
        tok = newToken(token.RPAREN, l.ch)
    case '{':
        tok = newToken(token.LBRACE, l.ch)
    case '}':
        tok = newToken(token.RBRACE, l.ch)
    case ';':
        tok = newToken(token.SEMICOLON, l.ch)
    case ',':
        tok = newToken(token.COMMA, l.ch)
    case '-':
        tok = newToken(token.MINUS, l.ch)
    case '!':
        tok = newToken(token.BANG, l.ch)
    case '*':
        tok = newToken(token.ASTERISK, l.ch)
    case '/':
        tok = newToken(token.SLASH, l.ch)
    case '<':
        tok = newToken(token.LT, l.ch)
    case '>':
        tok = newToken(token.GT, l.ch)
    case 0:
        tok.Literal = ""
        tok.Type = token.EOF
    default:
        if isLetter(l.ch) {
            tok.Literal = l.readIdentifier()
            tok.Type = token.LookupIdent(tok.Literal)
            return tok
        } else if isDigit(l.ch) {
            tok.Type = token.INT
            tok.Literal = l.readNumber()
            return tok
        } else {
            tok = newToken(token.ILLEGAL, l.ch)
        }
    }
    l.readChar()
    return tok 
}

func (l *Lexer) skipWhitespace() {
    for l.ch == ' ' || l.ch == '\t' || l.ch == '\n' || l.ch == '\r' {
        l.readChar()
    }
}

func (l *Lexer) readNumber() string {
    position := l.position
    for isDigit(l.ch) {
        l.readChar()
    }
    return l.input[position:l.position]
}

func isDigit(input byte) bool {
    return input >= '0' && input <= '9' 
}

func isLetter(input byte) bool {
    return input >= 'a' && input <= 'z' || input >= 'A' && input <= 'Z' || input == '_'
}

func (l *Lexer) readIdentifier() string {
    // store the current position
    position := l.position
    // check if the current character is a valid letter
    for isLetter(l.ch) {
        // if so, advance and read the next character
        l.readChar()
    }
    // return the slice of the identifier (basically a whole string, until our char is not a valid letter)
    // do this by slicing from the stored position at the beginning with the current position of the Lexer.
    return l.input[position:l.position]
}

// Creates a new token based on the string provided. The token has a type and the string literal.
func newToken(tokenType token.TokenType, ch byte) token.Token {
    return token.Token{Type: tokenType, Literal: string(ch)}
}
