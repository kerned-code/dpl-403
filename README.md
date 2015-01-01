dpl-403
=======

This started out as my Designer Programming Language project for Dr. Lusth’s CS 403 class. I expect to keep refining and expanding it.

My language does not yet have a name, partly because I lack the creativity to come up with something clever. I think of it as mostly like Scheme except for a much more idiosyncratic syntax (and not nearly as many features), but it also takes heavily after Dr. Lusth’s language [Scam](http://beastie.cs.ua.edu/scam/), and I developed the idea for interleaved function calls (more on those later) from a similar concept in AppleScript.

The Program
-----------

The language program is (thanks to Dr. Lusth) a relatively simple system of mostly independent layers.

### The Lexical Analyzer & Lexemes

At the base is the lexical analyzer (`lexer.c`), which accepts input files and converts the source code therein to lexemes, token by token. Lexemes themselves (`lexeme.c`) are just C structs that represent the various objects my language needs to know about in order to run. The lexemes’ types are string constants defined in the types module (`types.c`).

### The Parser & Parse Trees

The parser (`parser.c`) uses the lexer to read an input program and arranges the received lexemes into a parse tree if possible (and exits with an error message if not). The parse tree is a binary tree of lexemes. (The lexemes have left and right child pointers for this purpose.) The actual arrangement of the branches of the tree is delegated to the trees module (`trees.c`) so that the parser doesn’t have to know about the structural details of the subtrees; it just builds the trees using the functions provided by the trees module.

### Evaluation & Environments

After the parser, several additional modules and layers operate all at once. The coordinating module is the evaluator (`evaluator.c`), which takes input source code and attempts to execute the program represented by the code. In order to do this, the evaluator uses an environment module (`environment.c`) to store and retrieve program values that have been bound to names. The evaluator also depends on a separate module (`function-evaluator.c`) to execute calls to user-defined functions. Those evaluation functions are separate from the others because they are rather more complicated and require a number of helper functions. Calls to built-in functions are delegated to yet another module, discussed below.

### The Runner

The evaluator can be called with a single source file using the runner module (`runner.c`). The runner performs necessary initialization of the program environment before calling the evaluator. This includes construction of a starting environment, loading of the built-in library, generation of the parse tree, and evaluation of the program.

#### Built-In Functions
Unfortunately, the current system for managing built-in operations is rather complicated. Each operation has three components: a representative lexeme that stores its unique internal name (which I’ll call a “tag” here) and the names of its parameters; a helper function that performs the specified operation; and one or more names by which the function may be called in the program. Each of these components requires its own helper function.

A built-in operation is represented by a standard closure lexeme whose body includes the operation’s tag (in a lexeme with a type specific to built-ins). This structure is managed by the trees module mentioned previously. Tags themselves are stored in a dedicated module (`built-in-types.c`). Note that the lexeme does not store the operation’s in-language name(s). Because they are defined separately, a single operation may have multiple aliases, but it can only have one tag (internal name). Each operation has a helper function that returns a copy of its lexeme representation.

That lexeme forms the basis of the other features. A callable name for an operation is specified by attaching a copy of the lexeme to an identifier lexeme (again, handled by `trees.c`). A simple helper function is used for this purpose. All such helper functions are then registered in the loader module (`loader.c`), which is used to initialize the environment layer that hosts the built-in names for the language. The loader provides a function that loads all of the registered names into a given environment table. It is necessary to manually add each name’s helper function to the loader module.

A call to a built-in function is carried out by passing a built-in-type lexeme to the `execute` function in `built-ins.c`, which dispatches to the appropriate helper function by using the lexeme’s tag. Unfortunately, this means that it is necessary to manually associated each built-in operation with its execution function. The helper function itself uses the parameters specified in the operation’s lexeme to get the operation’s arguments from the active environment; the actual operation is then carried out (in C, of course), and the appropriate value is returned as a lexeme.

