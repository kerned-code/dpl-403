dpl-403
=======

This started out as my Designer Programming Language project for Dr. Lusth’s CS 403 class. I expect to keep refining and expanding it.

My language does not yet have a name, partly because I lack the creativity to come up with something clever. I think of it as mostly like Scheme except for a much more idiosyncratic syntax (and not nearly as many features), but it also takes heavily after Dr. Lusth’s language [Scam](http://beastie.cs.ua.edu/scam/), and I developed the idea for interleaved function calls (more on those later) from a similar concept in AppleScript.

The Program
-----------

The language program is (thanks to Dr. Lusth) a relatively simple system of mostly independent layers.

### The Lexical Analyzer & Lexemes

At the base is the lexical analyzer (lexer.c), which accepts input files and converts the source code therein to lexemes, token by token. Lexemes themselves (lexeme.c) are just C structs that represent the various objects my language needs to know about in order to run. The lexemes’ types are string constants defined in the types module (types.c).

### The Parser & Parse Trees

The parser (parser.c) uses the lexer to read an input program and arranges the received lexemes into a parse tree if possible (and exits with an error message if not). The parse tree is a binary tree of lexemes. (The lexemes have left and right child pointers for this purpose.) The actual arrangement of the branches of the tree is delegated to the trees module (trees.c) so that the parser doesn’t have to know about the structural details of the subtrees; it just builds the trees using the functions provided by the trees module.

### Evaluation & Environments

After the parser, several additional modules and layers operate all at once. The coordinating module is the evaluator (evaluator.c), which takes input source code and attempts to execute the program represented by the code. In order to do this, the evaluator uses an environment module (environment.c) to store and retrieve program values that have been bound to names. The evaluator also depends on a separate module (function-evaluator.c) to execute function calls. Those evaluation functions are separate from the others because they are rather more complicated and require a number of helper functions.

### The Runner

The evaluator can be called with a single source file using the runner module (runner.c). The runner performs necessary initialization of the program environment before calling the evaluator. This includes construction of a starting environment, loading of the built-in library, generation of the parse tree, and evaluation of the program.

#### Built-In Functions
This setup phase includes the loading of the built-in functions, by way of the loader module (loader.c). The built-ins module (built-ins.c) comprises a large collection of functions that perform the standard operations for my language. The built-in functions are identified by unique built-in types, as defined in built-in-types.c.

Unfortunately, the current system for managing built-in operations is rather complicated. 
