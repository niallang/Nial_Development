---
title: A Data Manipulation Language - Nial Introduction, Chapter 3
---


# A Data Manipulation Language
### Chapter 3

- [Functional Objects](#functional-objects)
- [Bracket Notation](#bracket-notation)
- [Addresses and Indexing](#addresses-and-indexing)

All data objects in Nial are treated as rectangular nested arrays with
dimen sionality, extent and depth. The array

    X := 2 3 reshape (8 9) 23 'hello world' "goodbye (tell 3 2)([sin,cos]0.5)
    
    +-----------------------------------+
    |8 9    |       23|hello world      |
    +-------+---------+-----------------|
    |goodbye|+-------+|0.479426 0.877583|
    |       ||0 0|0 1||                 |
    |       |+---+---||                 |
    |       ||1 0|1 1||                 |
    |       |+---+---||                 |
    |       ||2 0|2 1||                 |
    |       |+-------+|                 |
    +-----------------------------------+
    

is a 2 by 3 array of other arrays. The upper left entry (at address 0 0)
is the pair 8 9 and the item at address 1 1 is the table of addresses
for a 3 by 2 array. X holds a character string at address 0 2 and a
phrase at address 1 0.

There are three measurements of the structure of an array:

|                      |                                                       |
| -------------------- | ----------------------------------------------------- |
|  valence             | the number of dimensions (or axes)                    |
|  shape               | the list of lengths of the array along each dimension |
|  tally               | the number of items in the array.                    |


The measurements are related. For every array A, the following equations
hold:

    valence A = tally shape A
    tally A = product shape A

The definitions along with the second equation indicate that the term
item refers to top level objects in an array and not the most deeply
nested ones. The meaning of these measure ments for the array X is as
follows:

- the valence is 2 (X has two dimen sions),
- the shape is the list 2 3 (X has two rows and three columns), and
- the tally is 6 (X has six items).

The expression for the table assigned to X involves the infix use of
operation *reshape* between two lists, the first giving the shape of the
table and the second being a list of the arrays to be used as the items
of the table.

The syntax for a list is a sequence of two or more array expressions
placed side-by-side. This construct is called a strand. A strand
evaluates to the list that has as its items the values of the
expressions in the strand. An item of a strand can be an atomic array,
string or an expression in parentheses.

## Functional Objects

Nial has two classes of functional objects: operations and transformers.
Operations are the functional objects that map arrays to arrays, such as
*rows*, *reshape* and *link*. Operations are either predefined (implemented by
the Q'Nial interpreter) or defined by the user during a session.

Operations in Nial are said to be first order functions because they
act directly on an argument array to produce a result array. Since a
list of arrays is itself an array, some operations require that the
argument array have a specific number of items. As well, many operations
return a result that is a list of arrays computed by the operation.

The syntax for the use of an operation allows it to be used in both a
prefix and an infix manner. For example, operation *link* can be used
between two arguments that are lists in order to join the two lists
together, or it can be used in front of an argument that is a list of an
arbitrary number of lists in order to join all the lists together.

An operation is applied to an array and results in an array value. For
example, applying the operation *sum* to the list 3 4 5 results in the
value 12.

The notation for applying an operation in Nial is based on juxtaposition
(side-by-side placement of objects). The operation is placed before the
argument. The following example shows the juxtaposition syntax for
applying an operation.

         sum (3 4 5)
    12
    

The parentheses used in the above example are not necessary. Nial would
give the same result if they were omitted.

The notation for infix use of an operation like *reshape* places the
operation between two arguments. It is interpreted as the operation
applied to the pair formed by the two arguments.

         2 3 reshape 4 5 6 7 8 9
    4 5 6
    7 8 9
    

         reshape ((2 3) (4 5 6 7 8 9))
    4 5 6
    7 8 9
    

A transformer is a functional object that maps an operation to a
modified operation. It is said to be second order function, because it
is a functional object that operates on one or more first order
functions and produces a function. The effect of applying the
transformer *EACH* to the operation *first*, for example, results in an
modified operation (called the EACH transform of first) that applies
*first* to each item of its argument. Juxtaposition is used to denote the
application of a transformer to an operation. The application of the
modified operation in

         (EACH first) ((2 3) (4 5 6 7 8 9))
    2 4
    

is equivalent to applying first to the lists in the pair

        (first (2 3)) (first (4 5 6 7 8 9))
    2 4
    

Juxtaposition of objects can be used in three additional ways besides
its use in forming strands. Juxtaposition can occur in prefix use of an
operation, infix use of an operation and in application of a
transformer. To avoid ambiguity, specific rules have been adopted:

1.   Strand formation takes precedence over either infix or prefix operation application.
2.   Transformer application takes precedence over operation application.
3.   If two or more prefix uses of operations occur in a row, as in
     *sum link A*, the rightmost operation is done first and its result is used
     as the argument of the operation to the left.
4.   If an infix use precedes a prefix use, as in *A reshape link B*, 
     the prefix application is done first.
 
The first rule means that *sum 3 4 5* is the same as *sum (3 4 5)* and that
*2 3 link 4 5* is the same as *(2 3) link (4 5)*.

The second rule means that the expression *EACH first (2 3 4) (5 6 7 8 9)*
applies the modified operation *EACH first* to the pair formed by the
strand.

The third and fourth rules exist because there is no other sensible
interpretation of these juxtapositions.

A modified operation can also be used in an infix manner. For example,
in the operation labeltable described in the previous chapter, the
expression

    Rowlabels EACHBOTH hitch rows Table

uses *EACHBOTH hitch* in an infix manner between Rowlabels and the prefix
expression rows Tables.

The reading of the above expression is helped by the spelling convention
in Nial for names associated with the different classes of objects. An
operation is spelled in lower case, a transformer in upper case and an
array variable or named expression begins with a capital with the rest
in lower case. These rules make it possible to recognize the kind of
object associated with the name. The user of Q'Nial is not required to
spell names according to these rules. When definitions are displayed by
*see* or *defedit* they appear in canonical form with this prescribed
spelling format. Nial is case insensitive; *first* and *First* are both
legal spellings, but the former is the canonical one.

The interpretation of *sum link A* to mean *sum (link A)* given above
suggests that the juxtaposition sum link could be used to denote the
composition of operations *sum* and *link*, resulting in the following
equation:

    (sum link) A = sum (link A)

As well, it is useful to have this notation for composition in order to
specify the composition of two operations as the argument of a
transformer. For example:

    EACH (link second) A

The interpretation of the above rules is illustrated by the following
example:

    2 3 4 EACH first sum 4 5 6
    = (2 3 4) EACH first sum (4 5 6)          (strand rule)
    = (2 3 4)(EACH first) sum (4 5 6)         (transformer precedence)
    = (2 3 4)(EACH first)(sum (4 5 6))        (prefix operations rule)
    = (2 3 4)(EACH first) 15                  (meaning of sum)
    = (EACH first) ((2 3 4) 15)               (infix rule)
    = (first (2 3 4)) (first 15)              (meaning of each)
    = 2 15                                    (meaning of first)

A feature of Nial syntax is that all operations are implicitly unary and
their interpretation when used in an infix way is determined by the
above rules rather than by an operation precedence table. Thus,

    2 + 3 * 4
    = + (2 3) * 4
    = 5 * 4
    = * (5 4)
    = 20
    

This example demonstrates that infix uses of operations are evaluated
left to right without precedence. The same expression in Pascal or C
would be evaluated by doing the multiplication first.

## Bracket Notation

Nial syntax has a second way to construct a list. It is called
bracket-comma notation. The expression

    [34,275,86,-52]
    

has the same meaning as the strand

    34 275 86 -52
    

The bracket-comma notation has the advantage that it can represent lists
of length one and length zero. Thus,

    [235]
    

denotes the list of length one holding the atom 235 as its item and the
empty list Null is denoted by

    []
    

There is a corresponding syntactic object for operations. It is called
an atlas (a list of maps). An atlas applies each component operation to
the argument producing a list of results. Thus,

    [sin,cos,sqrt] 3.14
    

is equivalent to

    [sin 3.14,cos 3.14,sqrt 3.14]
    

An elegant example of atlas notation is to rewrite the definition of
average as:

    average IS / [sum,tally]

The equivalence with the earlier definition can be demonstrated as
follows:

    average A
    = (/ [sum,tally]) A
    = / ([sum,tally] A)                  (prefix operations rule)
    = / [sum A,tally A]                  (atlas rule)
    = / (sum A) (tally A)                (strand equivalence)
    = (sum A) / ( tally A)               (infix rule)
    

The atlas notation is mainly used as a shorthand for describing
operations without the need to explicitly name the argument. It is also
used to form an operation argument for a transformer that uses two or
more operations.

## Addresses and Indexing

In procedural programming languages, arrays are treated as subscripted
variables. A name declared to be an array is said to denote a collection
of variables, each of which can be assigned a value independently. In
such languages, there is a notation for denoting one variable from the
collection, similar to the idea of using a subscript in mathematical
notation. In such languages, array computations use looping constructs
to do a computation that accesses all the items of an array.

In Nial, arrays are implicitly given an addressing scheme. For the table
T, the addresses are given by the operation *grid* as an array of the same
shape as T, with each location holding its own address.

        T := 3 4 reshape count 12
    1  2  3  4
    5  6  7  8
    9 10 11 12
    

    grid T
    +---------------+
    |0 0|0 1|0 2|0 3|
    +---+---+---+---|
    |1 0|1 1|1 2|1 3|
    +---+---+---+---|
    |2 0|2 1|2 2|2 3|
    +---------------+
    

The addresses of an array are integers for a 1-dimensional array (a
list) or lists of integers for other arrays. The numbering scheme uses
zero-origin counting. For the table T above, the addresses are lists of
length two. The list A below has addresses that are integers.

         set "diagram;
         A := count 5
    +---------+
    |1|2|3|4|5|
    +---------+
    

         grid A
    +---------+
    |0|1|2|3|4|
    +---------+
    

The operation *pick* is the fundamental selection operation in Nial based
on addressing. The expression

         1 2 pick T
    7
    

selects the item of T at address 1 2. In picking from the list A, either
an integer or a list of one integer corresponding to an address can be
used:

         4 pick A
    5
    

         [4] pick A
    5
    

The operation *choose* can be used to select multiple items from an array.
For example, in the expression below, *choose* returns the items of T at
the three addresses in the left argument:

         [1 2,2 3,0 0] choose T
    7 12 1
    

Nial also has an indexing notation similar to subscript notation in
other languages. The following expression selects the item of T at
address 1 2.

         T@(1 2)
    7
    

This form of selection is called at-indexing. One difference between it
and using *pick* for selection is that *pick* can be modified using a
transformer because it is an operation but the @ symbol is a syntactic
construct and cannot be modified by a transformer. Also the at-notation
can only be used with a variable, whereas pick can select from the
result of any expression.
