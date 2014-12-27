/* Tucker Meyers: CS403 Programming Language Project
 * Lexeme Module
 * Each lexeme object represents a lexical token in the language.
 * It has a type, a value (corresponding to that type), and left 
 * and right pointers for forming lexeme trees.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexeme.h"
#include "types.h"

static Lexeme * newEmptyLexeme(void);
static int linkOnly(Lexeme * target);
static void alternate(Lexeme * mover, Lexeme * holder);
//static Lexeme * everyOther(Lexeme * list);


/*** Public Interface ***/

/* CONSTRUCTORS */

// This is the base form, to be used for valueless lexemes (punctuation etc.).
Lexeme * newLexeme(char * type) {
    Lexeme * L = newEmptyLexeme();
    L->type = type;  // Assume that the caller gives a valid type.
    return L;
}

Lexeme * newIntegerValueLexeme(long value) {
    Lexeme * L = newLexeme(INTEGER);
    L->intValue = value;
    return L;
}

Lexeme * newFloatValueLexeme(double value) {
    Lexeme * L = newLexeme(FLOAT);
    L->floatValue = value;
    return L;
}

Lexeme * newStringValueLexeme(char * value) {
    Lexeme * L = newLexeme(STRING);
    L->stringValue = value;
    L->intValue = strlen(value);
    return L;
}

Lexeme * newBooleanValueLexeme(int value) {
    Lexeme * L = newLexeme(BOOLEAN);
    L->booleanValue = (value != 0);  // Normalize 'Boolean' integers to zero and one.
    return L;
}

Lexeme * newIdentifierLexeme(char * value) {
    Lexeme * L = newLexeme(IDENTIFIER);
    L->stringValue = value;
    return L;
}

Lexeme * newArrayLexeme(int capacity) {
    Lexeme * L = newLexeme(ARRAY);
    L->array = malloc(capacity * sizeof(Lexeme));
    return L;
}

Lexeme * newBuiltInLexeme(char * function_keyword) {
    Lexeme * L = newLexeme(BUILT_IN);
    L->stringValue = function_keyword;
    return L;
}

/* LIST PROCESSING */

Lexeme * cons(Lexeme * left, Lexeme * right) {
    return consWithTag(NONDESCRIPT, left, right);
}

Lexeme * consWithTag(char * tag, Lexeme * left, Lexeme * right) {
    Lexeme * L = newLexeme(tag);
    setLeftChild(L, left);
    setRightChild(L, right);
    return L;
}

Lexeme * car(Lexeme * list) {
    return list->left;
}

Lexeme * cdr(Lexeme * list) {
    return list->right;
}

int length(Lexeme * list) {
    if (list != NULL)
        return 1 + length(cdr(list));
    else
        return 0;
}

void uninterleave(Lexeme * odds, Lexeme * evens) {
    /* Given two pointers to the same lexeme list, separate
     * the list into two sequences (one with the odd-indexed
     * elements and the other with the evens).
     */
    if (evens == NULL) 
        return;
    if (evens != NULL)
        alternate(odds, evens);
}

void setCar(Lexeme * parent, Lexeme * child) {
    setLeftChild(parent, child);
}

void setCdr(Lexeme * parent, Lexeme * child) {
    setRightChild(parent, child);
}

void setTail(Lexeme * tree, Lexeme * tail) {
    if (tree == NULL) {
        fprintf(stderr, "Tree error: Cannot set the tail of a null tree.\n");
        exit(-1);
    }
    else if (cdr(tree) == NULL)
        setCdr(tree, tail);
    else
        setTail(cdr(tree), tail);
    //printf("  setTail: "); printOut(car(tree));
}

Lexeme * copyLexemeTree(Lexeme * source) {
    Lexeme * L = newEmptyLexeme();
    // Copy values exactly.
    L->type = source->type;
    L->intValue = source->intValue;
    L->floatValue = source->floatValue;
    L->stringValue = source->stringValue;
    L->booleanValue = source->booleanValue;
    // Share an array pointer.
    L->array = source->array;
    // Some kinds of pointers must be linked/shared and not copied, 
    // such as environment tables and NULL pointers.
    if (linkOnly(source->left))
        L->left = source->left;
    else
        L->left = copyLexemeTree(source->left);
    if (linkOnly(source->right))
        L->right = source->right;
    else
        L->right = copyLexemeTree(source->right);
    return L;
}

/* ACCESSORS and MUTATORS */

char * getType(Lexeme * L) {
    return L->type;
}

int typeInteger(Lexeme * L) {return (getType(L) == INTEGER);}
int typeFloat(Lexeme * L) {return (getType(L) == FLOAT);}
int typeString(Lexeme * L) {return (getType(L) == STRING);}
int typeBoolean(Lexeme * L) {return (getType(L) == BOOLEAN);}
int typeArray(Lexeme * L) {return (getType(L) == ARRAY);}
int typePair(Lexeme * L) {return (getType(L) == PAIR);}
int typeIdentifier(Lexeme * L) {return (getType(L) == IDENTIFIER);}
int typeBuiltIn(Lexeme * L) {return (getType(L) == BUILT_IN);}

char * getIdentifierName(Lexeme * id) {
    if (typeIdentifier(id) || typeBuiltIn(id))
        return id->stringValue;
    else {
        fprintf(stderr, "Type error: Cannot get identifier name from type <%s>.\n", getType(id));
        exit(-1);
    }
}

long getIntegerValue(Lexeme * n) {
    if (typeInteger(n))
        return n->intValue;
    else {
        fprintf(stderr, "Type error: Cannot get integer value from type <%s>.\n", getType(n));
        exit(-1);
    }
}

double getFloatValue(Lexeme * n) {
    if (typeFloat(n))
        return n->floatValue;
    else {
        fprintf(stderr, "Type error: Cannot get floating-point value from type <%s>.\n", getType(n));
        exit(-1);
    }
}

char * getStringValue(Lexeme * l) {
    if (typeString(l))
        return l->stringValue;
    else {
        fprintf(stderr, "Type error: Cannot get string value from type <%s>.\n", getType(l));
        exit(-1);
    }
}

int getStringLength(Lexeme * l) {
    if (typeString(l))
        return l->intValue;
    else {
        fprintf(stderr, "Type error: Cannot get string length from type <%s>.\n", getType(l));
        exit(-1);
    }
}

int getBooleanValue(Lexeme * b) {
    if (typeBoolean(b))
        return b->booleanValue;
    else {
        fprintf(stderr, "Type error: Cannot get Boolean value from type <%s>.\n", getType(b));
        exit(-1);
    }
}

Lexeme * getArray(Lexeme * l) {
    if (typeArray(l))
        return l->array;
    else {
        fprintf(stderr, "Type error: Cannot get array value from type <%s>.\n", getType(l));
        exit(-1);
    }
}

Lexeme * getArrayValue(Lexeme * l, int index) {
    if (typeArray(l))
        return &(getArray(l)[index]);
    else {
        fprintf(stderr, "Type error: Cannot get array entries from type <%s>.\n", getType(l));
        exit(-1);
    }
}

Lexeme * setArrayValue(Lexeme * l, int index, Lexeme * value) {
    if (typeArray(l)) {
        getArray(l)[index] = *value;
        return l;
    }
    else {
        fprintf(stderr, "Type error: Cannot insert array entries in type <%s>.\n", getType(l));
        exit(-1);
    }
}

int getEnvironmentDepth(Lexeme * l) {
    if (getType(l) == ENVIRONMENT) {
        return l->intValue;
    }
    else {
        fprintf(stderr, "Type error: Cannot get depth of type <%s> (environments only).\n", getType(l));
        exit(-1);
    }
}

void setEnvironmentDepth(Lexeme * l, int depth) {
    if (getType(l) == ENVIRONMENT) {
        l->intValue = depth;
    }
    else {
        fprintf(stderr, "Type error: Cannot set depth of type <%s> (environments only).\n", getType(l));
        exit(-1);
    }
}

void printOut(Lexeme * L) {
    if (L == NULL) 
        printf("NULL (no lexeme)");
    else {
        printf("%s ", getType(L));
        if (typeInteger(L))
            printf("%ld", L->intValue);
        else if (typeFloat(L))
            printf("%16.15f", L->floatValue);
        else if (typeString(L) || typeIdentifier(L) || typeBuiltIn(L))
            printf("%s", L->stringValue);
        else if (typeBoolean(L))
            printf("%s", L->booleanValue ? "true" : "false");
    }
    printf("\n");
}

void setLeftChild(Lexeme * parent, Lexeme * child) {
    parent->left = child;
}

void setRightChild(Lexeme * parent, Lexeme * child) {
    parent->right = child;
}


/*** Private Methods ***/

static Lexeme * newEmptyLexeme() {
    Lexeme * L = (Lexeme *) malloc(sizeof(Lexeme));
    if (L == 0) {
        fprintf(stderr, "Lexeme allocation failed; insufficient memory available.");
        exit(-1);
    }
    L->left = NULL;
    L->right = NULL;
    return L;
}

static int linkOnly(Lexeme * target) {
    // Null lexemes and environment-related lexemes should
    // not be copied during lexeme copying; instead, they
    // should be linked (shared).
    return (target == NULL
        || getType(target) == ENVIRONMENT
        || getType(target) == TABLE
    );
}

static void alternate(Lexeme * mover, Lexeme * holder) {
    /* Assign the cdr of the holder to the cdr of the mover and
     * advance the mover up the list, then repeat with the lists
     * in reversed roles until the end of the list is reached.
     */
    if (mover == NULL || holder == NULL) 
        return;
    setCdr(mover, cdr(holder));
    mover = cdr(mover);
    alternate(holder, mover);
}

