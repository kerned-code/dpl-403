# Tucker Meyers: CS403 Programming Language Project
# Recognizer Module Read-Me

Files in this module:
 - types.h, types.c
 - lexeme.h, lexeme.c
 - lexer.h, lexer.c
 - trees.h, trees.c
 - parser.h, parser.c
 - environment.h, environment.c
 - evaluator.h, evaluator.c
 - function-evaluator.h, function-evaluator.c
 - built-in-types.h, built-in-types.c
 - built-ins.h, built-ins.c
 - loader.h, loader.c
 - runner.c
 - makefile
 - test files
 - grammar.txt

NOTES:

Although I believe that it meets all of the stated requirements, my language is
not "complete". In particular:
 - There is not support for 'for' loops, but 'while' loops remain. My initial 
   approach to for loops with iterators defied my attempts to implement it, and 
   I have not had time to devise a new strategy.
 - I have defined several dozen built-in functions, but not all of them have
   been tested, and a handful are possibly not yet complete. In particular,
   my string coercions for arrays and closures are not complete--they return
   generic messages like "<an array>" or "<a closure>".

    There is some stylistic inconsistency between modules. In particular, I 
switched from camel-case function names to underscore-delimited function names
at some point. I have left this inconsistency in place and have continued to
follow it, so new functions in old modules are still in camel case. Although 
this makes reading awkward on occasion, at least it marks clearly when a new
module calls an old one.

    My language has no name. Source files are named with the ending '.source',
though of course the interpreter doesn't care what the file ending is.

BUILDING AND RUNNING:

    The makefile responds to 'make' and 'make build' to compile the module into
a single executable called 'runner'. The following commands are also recognized,
as mandated:
 - arrays, source-arrays
 - conditionals, source-conditionals,
 - functions, source-functions
 - iteration, source-iteration
 - lists, source-lists
 - objects, source-objects
 - recursion, source-recursion
 - clean
Finally, you can run the calculator by calling 'runner calculator.source' or with
the make command 'make rpn'.

FEATURES:

    My language includes the features required: comments, integers (and floating-
point numbers), strings, dynamically-typed variables, objects, arrays (with 
constant access time), conditionals, recursion, iteration, printing, operators, and
first-class function objects.

    My language also supports interleaved function calls, the big, "novel" feature
that was the focus of my work.
 - A function with the name "f" must be called as (f: ...).
 - A function with the name "f_" can be called as (f_: ...), or as (f ...) if it has 
     exactly one parameter.
 - A function with the name "_f_" can be called as (_f_: ...), or as (... f ...) if
     it has exactly two parameters.
 - Similarly, "_f_g_" with three parameters can be (... f ... g ...). The only limits
     on interleaving are (1) the limit on the length of literal identifiers, which is
     512 characters, and (2) the practical bounds of memory space. (Looking up
     interleaved names requires considerable string manipulation in the background. 
     I had forgotten how much I really don't like dealing with strings in C.)
 - Finally, a binary operator with the name "_op_&" may be embedded with other infix
     calls in a series. This form is known as an "extensible" function. 
     Consider "_f_g_" above:
     (.. op .. op .. f .. g ..) = (((.. op ..) op ..) f .. g ..)

Some of these are difficult to explain clearly, but there are examples of them in
my sample source code.

    Each scope is pre-loaded with the special variable "@", which points to the local
environment itself. It is also secretly inserted as the "return" value for class 
closures, and its value can be used for get/set operations. This name is not reserved.

BUILT-IN LIBRARY:

Types:
    (type: x) returns a string telling the type of the given element. (I would like
        to reimplement this with symbols later, but strings will do for now.)
    A series of predicates--null?, int?, float?, string?, boolean?, array?, pair?
        (and list?), identifier?, closure?, scope?--return Boolean values as expected.
Scopes:
    (get: scope name) takes a scope object and a string and returns the bound value
        if there is one; if not, or if the value is nil, it returns nil. I'd really
        like to be able to use symbols here, because strings feel clunky.
    (set: scope name value) takes a scope object, a string, and a value and binds
        the name to the new value.
    Infix aliases: 
        (scope . name) = (get: scope name)
        (scope . name set value) = (set: scope name value)
Arithmetic:
    There are five standard operators: (+: a b), (-: a b), (*: a b), (/: a b), (^: a b)
        Division is floor division for integers and decimal for floats, as in C.
    Each of the five has a corresponding extensible infix alias, so the five may be
        mixed in any order, keeping in mind that evaluation is strictly left associative.
            (1 + 2 + 3 + 4) is 10
            (1 + 3 / 2 - 4) is (4 / 2 - 4) is (2 - 4) is -2
Numeric comparison:
    There are six standard operators: = != > >= < <=
    Each has an infix alias.
Boolean logical operations:
    There are three standard operators: (not: a) (and: a b) (or: a b)
    Aliases:
        (not a) = (not: a)
        (a and b) = (and: a b)
        (a or b) = (or: a b)
List operations:
    Form pairs or cells with the command (pair: left right) or its infix
        alias (left , right). Note that, because the comma is an identifier,
        the spaces on both sides are necessary.
    Access pair members with (left: p) and (right: p).
    Rebind pair members with (set-left: p left) and (set-right: p right).
Array operations:
    Construct an array with N elements: (array: N).
    Access an array element: (array-get: a N).
    Rebind an array element: (array-set: a N value).
String operations:
    Access a character by index: (char: s N).
    Concatenate: (concat: sleft sright).
    Compare (true if the two strings contain the same characters in the same
        order, false otherwise): (compare: s1 s2).
Input/output operations;
    (print: message) and (println: message) coerce their arguments to strings
        if possible and print them as described.
    (read-terminal: N) reads N characters (from 1 to 512) from the console
        as a string.

The built-in names are not reserved, so they may be rebound. Also, it is easy to write
aliases (including interleaved and extensible ones) for these built-ins in case the 
provide syntax is undesirable. This is true of all functions of course. The only off-
limits words are the reserved words for language-primitive operations (bind, rebind, 
closure, function, class, and so on).

