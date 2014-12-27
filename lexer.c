/* Tucker Meyers: CS403 Programming Language Project
 * Lexer Module
 * The lexer class provides methods for lexical analysis of 
 * source code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "lexeme.h"
#include "types.h"

static int show_debugging_messages = 0;

static Lexer * newEmptyLexer(void);
static void nextLine(Lexer * L);
static char readNext(Lexer * L);
static void pushBack(Lexer * L, char c);
static void skipInvisibles(Lexer * L);
static int isPunctuation(char c);
static int isQuoteMark(char c);
static void consumeComment(Lexer * L);
static void checkNextCharacter(Lexer * L, char * type);
static Lexeme * lexNumber(Lexer * L, char c);
static Lexeme * lexString(Lexer * L, char c);
static Lexeme * lexIdentifier(Lexer * L, char c);


/*** Public Interface ***/

Lexer * newLexer(FILE * fp) {
    Lexer * L = newEmptyLexer();
    L->sourceFile = fp;
    L->currentLine = 1;
    return L;
}

void initializeLexer(Lexer * L) {
    /* Set up this lexer to keep track of lexemes, such as for parsing. */
    L->currentLexeme = lex(L);
}

int getCurrentLineNumber(Lexer * L) {
    return L->currentLine;
}

Lexeme * lex(Lexer * L) {
    skipInvisibles(L);
    char c = readNext(L);
    /* End of file: */
    if (c == EOF) 
        return newLexeme(END_OF_INPUT);
    /* Single-character tokens: */
    if (c == '(')
        return newLexeme(LEFT_PARENTHESIS);
    else if (c == ')')
        return newLexeme(RIGHT_PARENTHESIS);
    else if (c == '[')
        return newLexeme(LEFT_BRACKET);
    else if (c == ']')
        return newLexeme(RIGHT_BRACKET);
    else if (c == '{')
        return newLexeme(LEFT_BRACE);
    else if (c == '}')
        return newLexeme(RIGHT_BRACE);
    else if (c == ':')
        return newLexeme(COLON);
    /* Strings: */
    else if (isQuoteMark(c))
        return lexString(L, c);
    /* Numbers: */
    else if (isdigit(c))
        /* Only positive numbers in this case.
         * Also notice that decimal numbers must begin with at least one digit.
         */
        return lexNumber(L, c);
    /* Everything else: */
    else if (c == '-') {
        /* Might be negative number, might be identifier. */
        char d = readNext(L);
        if (isdigit(d)) {
            pushBack(L, d);
            return lexNumber(L, c);
        }
        else {
            pushBack(L, d);
            return lexIdentifier(L, c);
        }
    }
    else {
        return lexIdentifier(L, c);
    }
}

int check(Lexer * L, char * type) {
    return (L->currentLexeme->type == type);
}

Lexeme * match(Lexer * L, char * type) {
    Lexeme * current = getCurrentLexeme(L);
    require(L, type);
    advance(L);
    return current;
}

void require(Lexer * L, char * type) {
    if (! check(L, type))
        typeError(L, type);
    if (show_debugging_messages)
        printOut(getCurrentLexeme(L));
}

void typeError(Lexer * L, char * expected) {
    printf("illegal\n");
    fprintf(stderr, "Type error: Expected type <%s> but found <%s> (line %d).\n",
        expected, getType(getCurrentLexeme(L)), getCurrentLineNumber(L));
    exit(-1);
}

void advance(Lexer * L) {
    L->currentLexeme = lex(L);
}

Lexeme * getCurrentLexeme(Lexer * L) {
    return L->currentLexeme;
}


/*** Private Functions ***/

static Lexer * newEmptyLexer() {
    Lexer * L = (Lexer *) malloc(sizeof(Lexer));
    if (L == 0) {
        fprintf(stderr, "Lexer allocation failed; insufficient memory available.");
        exit(-1);
    }
    return L;
}

static void nextLine(Lexer * L) {
    L->currentLine += 1;
}

static char readNext(Lexer * L) {
    return fgetc(L->sourceFile);
}

static void pushBack(Lexer * L, char c) {
    ungetc(c, L->sourceFile);
    return;
}

static void skipInvisibles(Lexer * L) {
    char c = readNext(L);
    while (1) {
        if (c == EOF) {
            /* Let the caller handle the EOF. */
            pushBack(L, c);
            break;
        }
        else if (isspace(c)) { 
            /* Consume spaces, keeping track of the line count as we go. */
            if (c == '\n')
                nextLine(L);
        }
        else if (c == '#') {
            /* Comments always begin with #. */
            consumeComment(L);
        }
        else {
            /* Everything else is handled by the caller. */
            pushBack(L, c);
            break;
        }
        c = readNext(L);
    }
    return;
}

static int isPunctuation(char c) {
    switch (c) {
        case '(':
        case ')':
        case '[':
        case ']':
        case '{':
        case '}':
        case ':':
        case ',':
        case '#':     /* also catches comments */
            return 1;
    }
    return 0;
}

static int isQuoteMark(char c) {
    return (c == '\'' || c == '\"');
}

static int stringsMatch(char * a, char * b) {
    /* Compare the two strings wihout regard for case. */
    if (strlen(a) != strlen(b))
        return 0;
    while (*a) {
        if (tolower(*a) != tolower(*b))
            return 0;
        a++;
        b++;
    }
    return 1;
}

static void consumeComment(Lexer * L) {
    /* Block comments are delimited by #: and :#.
     * End-of-line comments are preceded by #.
     *
     * If an EOF is read, this function will push it back into the stream
     * so that the caller can perform end-of-input operations.
     *
     * It is assumed that the initial # was the last character read from the stream.
     */
    char c = readNext(L);
    if (c == EOF) {
        pushBack(L, c);
        return;
    }
    /* Block comment */
    if (c == ':') {
        c = readNext(L);
        if (c == EOF) {
            pushBack(L, c);
            return;
        }
        if (c == '\n')
            nextLine(L);
        /* Store two characters at a time until end marker :# is found. */
        char n = readNext(L);
        while (1) {
            if (n == EOF) {
                /* An unclosed block comment (essentially a "file-end" comment) is allowed. */
                pushBack(L, c);
                break;
            }
            /* Count line breaks inside comments. */
            if (n == '\n')
                nextLine(L);
            
            if (c == ':' && n == '#')
                break;
            c = n;
            n = readNext(L);
        }
    }
    /* End-of-line comment */
    else {
        while (c != '\n') {
            c = readNext(L);
        }
        nextLine(L);
    }
    return;
}

static void checkNextCharacter(Lexer * L, char * type) {
    /* If the next character is allowed to immediately follow a literal 
     * (such as a number or string), put the character back and return.
     * Otherwise, show an error and exit.
     * Allowed characters include whitespace, EOF, and punctuation.
     * This rule prevents things like 123identifier and "string"456,
     * but it also rules out 123op456; i.e., all function calls, including
     * to "operators", must be space-delimited.
     */
    char c = readNext(L);
    /* Check spaces/EOF. Call isspace (not isWhiteSpace) to avoid double-counting newlines. */
    if (isspace(c) || c == EOF || isPunctuation(c)) {
        pushBack(L, c);
        return;
    }
    fprintf(stderr, "Format error: Disallowed character <%c> following token of type '%s' (line %d).\n", 
        c, type, getCurrentLineNumber(L));
    exit(-1);
}

static Lexeme * lexNumber(Lexer * L, char c) {
    /* Handles both integers and floating-point numbers. */
    /* Numbers are limited to sixteen digits, which should usually
     * prevent string-to-double conversion errors. Space is also
     * allocated for an optional decimal point and/or negative sign.
     * Also, integers are limited to nine digits to allow smooth
     * string-to-long conversion on 32-bit systems.
     */
    int countDigits = 0;
    int nextSlot = 0;
    int decimalPoint = -1;  /* Keep track of the location of the optional decimal point. */
    int isNonzero = 0;      /* This is for checking conversion success/failure. */
    char digits[19];        /* Space for 16 digits, negative sign, decimal point, and \0 */
    digits[nextSlot] = c;
    nextSlot++;
    if (isdigit(c)) {
        countDigits++;
        if (c != '0')
            isNonzero = 1;
    }
    c = readNext(L);
    while (1) {
        if (isdigit(c)) {
            /* Disallow numbers with more than sixteen digits. */
            if (countDigits == 16) {
                fprintf(stderr, "Number format error: Too many digits (line %d).\n", 
                    getCurrentLineNumber(L));
                exit(-1);
            }
            if (c != '0')
                isNonzero = 1;
            countDigits++;
            digits[nextSlot] = c;
            nextSlot++;
        }
        else if (c == '.') {
            /* Only allow one decimal point, of course. */
            if (decimalPoint != -1) {
                fprintf(stderr, "Number format error: Too many decimal points (line %d).\n", 
                    getCurrentLineNumber(L));
                exit(-1);
            }
            decimalPoint = nextSlot;
            digits[nextSlot] = c;
            nextSlot++;
        }
        else {
            pushBack(L, c);
            break;
        }
        c = readNext(L);
    }
    /* Make sure that the number is not followed immediately
     * by an illegal symbol, as in 123abc or 456"string".
     */
    checkNextCharacter(L, "number");
    /* Perform conversion */
    digits[nextSlot] = '\0';
    /* Integer: */
    if (decimalPoint == -1) { 
        if (countDigits > 9) {
            fprintf(stderr, "Number format error: Too many digits for integer (line %d).\n", 
                getCurrentLineNumber(L));
            exit(-1);
        }
        long l = atol(digits);
        if (l == 0 && isNonzero) {
            fprintf(stderr, "Number format error: Conversion failed (line %d). Input: %s. Output: %ld.\n", 
                getCurrentLineNumber(L), digits, l);
            exit(-1);
        }
        return newIntegerValueLexeme(l);
    }
    /* Floating-point number: */
    else {
        double f = atof(digits);
        if (f == 0.0 && isNonzero) {
            fprintf(stderr, "Number format error: Conversion failed (line %d). Input: %s. Output: %f.\n",
                getCurrentLineNumber(L), digits, f);
            exit(-1);
        }
        return newFloatValueLexeme(f);
    }
}

static Lexeme * lexString(Lexer * L, char c) {
    /* Strings may be delimited by pairs of single or double quotation 
     * marks (but never mixed). Escape sequences: \n, \t, \", \', \\
     * Quotes of opposite types need not be escaped (i.e., a double-quoted
     * string may contain single quotes without escaping).
     */
    /* Save the opening quote mark for matching. */
    char quote = c;
    char * buffer;
    buffer = malloc(sizeof(char) * 1025);  /* Enough space for 1024 characters and \0. */
    int i = 0;
    /* Skip the quotation mark. */
    c = readNext(L);
    while (c != quote) {
        if (c == EOF) {
            fprintf(stderr, "String format error: Reached end of file before end of string (line %d).\n", 
                getCurrentLineNumber(L));
            exit(-1);
        }
        /* Strings are limited to 1024 characters. */
        if (i == 1023) {
            fprintf(stderr, "String format error: Too many characters (line %d).\n", 
                getCurrentLineNumber(L));
            exit(-1);
        }
        /* Allow literal newlines in strings, and count them like any other line in the file. */
        if (c == '\n')
            nextLine(L);
        /* Handle escape sequences: */
        if (c == '\\') {
            c = readNext(L);
            if (c == 'n')
                buffer[i] = '\n';
            else if (c == 't')
                buffer[i] = '\t';
            else if (c == '\"' || c == '\'' || c == '\\')
                buffer[i] = c;
            else {
                fprintf(stderr, "String format error: Unrecognized escape sequence (line %d): \"\\%c\".\n", 
                    getCurrentLineNumber(L), c);
                exit(-1);
            }
        }
        else {
            /* Not an escape sequence */
            buffer[i] = c;
        }
        i++;
        c = readNext(L);
    }
    buffer[i] = '\0';
    checkNextCharacter(L, "string");
    return newStringValueLexeme(buffer);
}

static Lexeme * lexIdentifier(Lexer * L, char c) {
    /* Identifiers are mostly-arbitrary strings of characters.
     * This also includes keywords such as "for", "while", "function",
     * "true", and "false".
     * 
     * There are not many conditions to check since lexIdentifier is only
     * called after all other options have been ruled out (i.e., punctuation,
     * numeric literals, strings, etc.).
     */
    char * buffer;
    buffer = malloc(sizeof(char) * 513);  /* Enough space for 512 characters and \0. */
    int i = 0;
    buffer[i] = c;
    i++;
    c = readNext(L);
    while (1) { 
        /* Spaces, punctuation, etc. signal the end of the identifier. */
        if (isspace(c) || c == EOF || isPunctuation(c)) {
            pushBack(L, c);
            break;
        }
        /* Quotation marks are not allowed in or immediately following identifiers. */
        if (isQuoteMark(c)) {
            fprintf(stderr, "Identifier error: Quotation mark in identifer (line %d).\n",
                getCurrentLineNumber(L));
            exit(-1);
        }
        /* Identifiers of more than 512 characters are disallowed. And poor style. */
        if (i == 511) {
            fprintf(stderr, "Identifier error: Too many characters (line %d).\n", 
                getCurrentLineNumber(L));
            exit(-1);
        }
        buffer[i] = c;
        i++;
        c = readNext(L);
    }
    buffer[i] = '\0';
    /* Check for keywords. */
    if (stringsMatch(buffer, "closure"))
        return newLexeme(KEYWORD_CLOSURE);
    else if (stringsMatch(buffer, "with"))
        return newLexeme(KEYWORD_WITH);
    else if (stringsMatch(buffer, "function"))
        return newLexeme(KEYWORD_FUNCTION);
    else if (stringsMatch(buffer, "class"))
        return newLexeme(KEYWORD_CLASS);
    else if (stringsMatch(buffer, "bind"))
        return newLexeme(KEYWORD_BIND);
    else if (stringsMatch(buffer, "rebind"))
        return newLexeme(KEYWORD_REBIND);
    else if (stringsMatch(buffer, "if"))
        return newLexeme(KEYWORD_IF);
    else if (stringsMatch(buffer, "else"))
        return newLexeme(KEYWORD_ELSE);
    else if (stringsMatch(buffer, "while"))
        return newLexeme(KEYWORD_WHILE);
    //else if (stringsMatch(buffer, "for"))
    //    return newLexeme(KEYWORD_FOR);
    else if (stringsMatch(buffer, "true"))
        return newBooleanValueLexeme(TRUE);
    else if (stringsMatch(buffer, "false"))
        return newBooleanValueLexeme(FALSE);
    //else if (stringsMatch(buffer, "self"))
    //    return newLexeme(SELF);
    else if (stringsMatch(buffer, "nil"))
        return newLexeme(NIL);
    /* Not a keyword. */
    else
        return newIdentifierLexeme(buffer);
}

