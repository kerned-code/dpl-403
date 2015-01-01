## `DPL 403`

This started out as my Designer Programming Language project for Dr. Lusth’s CS 403 class. I expect to keep refining and expanding it.

My language does not yet have a name, because I haven’t had the motivation come up with something clever yet. I think of it as mostly like Scheme except for a much more idiosyncratic syntax (and not nearly as many features), but it also takes heavily after Dr. Lusth’s language [Scam](http://beastie.cs.ua.edu/scam/), and I developed the idea for interleaved function calls (more on those later) from a similar concept in AppleScript.

## The Program

The language program is (thanks to Dr. Lusth) a relatively simple system of mostly independent layers.

#### The Lexical Analyzer & Lexemes

At the base is the lexical analyzer (`lexer.c`), which accepts input files and converts the source code therein to lexemes, token by token. Lexemes themselves (`lexeme.c`) are just C structs that represent the various objects my language needs to know about in order to run. The lexemes’ types are string constants defined in the types module (`types.c`).

#### The Parser & Parse Trees

The parser (`parser.c`) uses the lexer to read an input program and arranges the received lexemes into a parse tree if possible (and exits with an error message if not). The parse tree is a binary tree of lexemes. (The lexemes have left and right child pointers for this purpose. In this program, “tree” and “lexeme” are effectively synonymous, and I will use the terms interchangeably as context dictates.) The actual arrangement of the branches of the tree is delegated to the trees module (`trees.c`) so that the parser doesn’t have to know about the structural details of the subtrees; it just builds the trees using the functions provided by the trees module.

#### Evaluation & Environments

After the parser, several additional modules and layers operate all at once. The coordinating module is the evaluator (`evaluator.c`), which takes input source code and attempts to execute the program represented by the code. In order to do this, the evaluator uses an environment module (`environment.c`) to store and retrieve program values that have been bound to names and to keep track of scopes as the program is executed. The evaluator also depends on a separate module (`function-evaluator.c`) to handle calls to user-defined functions. Those evaluation functions are separate from the others because they are rather more complicated and require a number of helper functions. Calls to built-in functions are delegated to yet another module.

##### Built-In Functions
The built-ins module (`built-ins.c`) contains the definitions of all of the language’s native operations. Each one has a lexeme representation analogous to the closure lexemes for user-defined functions, one or more assigned names, and an execution function that carries out the actual operation. The built-ins module provides an `execute` function that, given a built-in-function lexeme, dispatches the appropriate helper function to perform the operation.

Each operation’s lexeme tree must contain two basic elements: a unique internal identifier (these are defined in `built-in-types.c`) and a list of parameters. The identifier is used to dispatch the correct helper function, which then uses the names in the parameter list to retrieve the function’s arguments from the active environment table. The tree is defined by a function in the built-ins module.

To be user-accessible, each operation must have at least one name. The functions’ names are defined separately from their lexeme representations, as functions in the built-ins module. (Note that the exact implementation of this system is likely to change in the near future, but broadly it involves wrapping the original lexeme object in another tree with an identifier lexeme in it.) The named object is then registered with the loader module (`loader.c`), which provides a function that inserts all of the built-in names with their values to a given environment table.

#### The Runner

The evaluator can be called with a single source file using the runner module (`runner.c`). The runner performs necessary initialization of the program environment before calling the evaluator. This includes construction of a starting environment, loading of the built-in library, generation of the parse tree, and evaluation of the program.


-----

## The Language

[Details to come; consult `grammar.txt` in the meantime.]
