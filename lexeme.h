/* Tucker Meyers: CS403 Programming Language Project
 * Lexeme Module
 */

#ifndef LEXEME_HEADER
#define LEXEME_HEADER

typedef struct lexeme {
    // Lexeme type:
    char * type;
    // Values:
    long intValue;  // All integers are stored as long integers.
    double floatValue;
    char * stringValue;
    int booleanValue;  // Values: 0 is false; anything else (preferably, 1) is true.
    struct lexeme * array;
    // Tree structure pointers:
    struct lexeme * left;
    struct lexeme * right;
} Lexeme;

/* Constructors: */
extern Lexeme * newLexeme(char * type);
extern Lexeme * newIntegerValueLexeme(long value);
extern Lexeme * newFloatValueLexeme(double value);
extern Lexeme * newStringValueLexeme(char * value);
extern Lexeme * newIdentifierLexeme(char * value);
extern Lexeme * newBooleanValueLexeme(int value);
extern Lexeme * newArrayLexeme(int capacity);
extern Lexeme * newBuiltInLexeme(char * function_name);

/* List-processing functions: */
extern Lexeme * cons(Lexeme * Left, Lexeme * right);
extern Lexeme * consWithTag(char * tag, Lexeme * Left, Lexeme * right);
extern Lexeme * car(Lexeme * List);
extern Lexeme * cdr(Lexeme * List);
extern int length(Lexeme * List);
extern void uninterleave(Lexeme * odds, Lexeme * evens);
//extern Lexeme * everyOtherFromFirst(Lexeme * source, Lexeme target);
//extern Lexeme * everyOtherFromSecond(Lexeme * source, Lexeme target);
extern void setCar(Lexeme * parent, Lexeme * child);
extern void setCdr(Lexeme * parent, Lexeme * child);
extern void setTail(Lexeme * tree, Lexeme * tail);
extern Lexeme * copyLexemeTree(Lexeme * source);

/* Other public methods: */
extern char * getType(Lexeme * L);
extern int typeInteger(Lexeme * L);
extern int typeFloat(Lexeme * L);
extern int typeString(Lexeme * L);
extern int typeBoolean(Lexeme * L);
extern int typeArray(Lexeme * L);
extern int typePair(Lexeme * L);
extern int typeIdentifier(Lexeme * L);
extern char * getIdentifierName(Lexeme * L);
extern long getIntegerValue(Lexeme * L);
extern double getFloatValue(Lexeme * L);
extern char * getStringValue(Lexeme * L);
extern int getStringLength(Lexeme * L);
extern int getBooleanValue(Lexeme * L);
extern Lexeme * getArray(Lexeme * L);
extern Lexeme * getArrayValue(Lexeme * L, int index);
extern Lexeme * setArrayValue(Lexeme * L, int index, Lexeme * value);
extern int getEnvironmentDepth(Lexeme * L);
extern void setEnvironmentDepth(Lexeme * L, int depth);
extern void printOut(Lexeme * L);
extern void setLeftChild(Lexeme * parent, Lexeme * child);
extern void setRightChild(Lexeme * parent, Lexeme * child);

#endif

