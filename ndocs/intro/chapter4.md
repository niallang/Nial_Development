---
title: Language Mechanisms - Nial Introduction, Chapter 4
layout: single
---


# Language Mechanisms

- [Assignment Expressions](#assignment-expressions)
- [Selector Expressions](#selector-expressions)
- [Iterations](#iterations)
- [Expression Sequences](#expression-sequences)
- [Blocks](#blocks)
- [Defining a Parameterized Operation](#defining-a-parameterized-operation)
- [Defining a Transformer](#defining-a-transformer)

Nial has many linguistic mechanisms that are similar to those used in
other programming languages. These can be group ed under a small number
of headings: assignments, selections, iterations and function
construction.

## Assignment Expressions

Assignment has been illustrated in many of the previous examples. An
assign-expression associates a name or a list of names to a value. There
are three forms involving a single variable, a sequence of variables, or
an indexed variable as the target:

    X gets count 20;
    A B C := 10 20 30;
    Y@3 := tell X@2;
    

In the first example, the keyword *gets* is used between the target and
the value being assigned. In the second two examples, the alternative
symbol *:=* is used. In the second form, using a sequence of variables as
the target, the value on the right must have the same tally as the
number of target variables on the left. The example showing this is
valid because three variables are assigned with a list of length three.
In the third example, the at-indexing notation is being used on the
right to select a value from X and on the left to insert a value into Y.

For the first two forms, the result value is the value on the right. For
the third form, the value of the result of the assign-expression is the
value of the updated variable.

## Selector Expressions

There are two selector mechanisms: if-expressions and case-expressions.
The if-expression can have a single conditional expression or a sequence
of conditional expressions. An optional else clause can be provided.
Examples are:

    X gets IF A = 0 THEN
      "Infinity
    ELSE
      Y / abs A
    ENDIF;
    

in which an if-expression is used to select between values, and

    IF A=0 THEN
      X:=?undefined ;
    ELSEIF A < 0 THEN
      X:=Y / opposite A;
    ELSE
      X:=Y / A;
    ENDIF ;
    write link 'X is ' (string X);
    

in which a selection is made among three expression sequences. In the
first example, semicolons are not placed after the expressions in the
THEN and ELSE clauses so that the values of the expressions will be
passed as the value of the entire if-expression, which is assigned to
variable X. In the second example, each clause is an expression
sequence that makes an assignment to variable X. In this case, no
overall result is expected and semicolons appear after each clause.

A case-expression is also used to choose among values or actions. A
simple example is:

    X := CASE N FROM
      1: sin Y END
      2: cos Y END
      3: tan Y END
      ELSE
        fault 'unexpected value of N
      ENDCASE;
      

The value of the expression after the keyword CASE is used to select one
of the clauses following the FROM. The values preceding the colons are
unique constants that are compared with the selection value. Only one of
the expressions is evaluated.

## Iterations

There are three forms of iteration in Nial: while-expressions,
repeat-until expressions and for-expressions. Each implements an
explicit looping construct.

The while-loop and repeat-loop are similar to the same constructs in
other languages. They are useful when there is an iteration in which it
cannot be predicted ahead of time as to how many times the loop needs to
be traversed.

This happens, for example, when reading from a file of unknown length,
determining the value of a converging series, or examining a data
structure of unknown structure. A loop to process the records of a file
using sequential file operations can be imple mented in Nial as

    File := open "myfile "r;
    Record := readfile File;
    WHILE Record ~= ??eof DO
      <process record>
      Record := readfile File;
    ENDWHILE;
    close File;
    

The for-loop in Nial differs from the for-loop concept used in most
compiled languages in that the set of values used in the iteration is
determined at the beginning of the loop, fixing the number of times the
body will be evaluated. In the following example, the loop evaluates
count 20 to the list of the first 20 integers. The body is then
evaluated 20 times, first with one, then with two, etc.

    FOR I WITH count 20 DO
      X@I := I*I;
    ENDFOR
    

There is no restriction on the values used in the control array. The
above loop to process the records of a file could also be programmed as

    Records := getfile "myfile;
    FOR Record with Records DO
      <process record>
    ENDFOR;
    

In this case the operation *getfile* returns a list of character strings
containing the records of the file. Each is processed in turn. The
advantage of the first version using the while-expression is that it
requires less internal storage since only one record of the file is in
the workspace at a time.

## Expression Sequences

All the control constructs for iterations and selections have two kinds
of expressions in them: expressions that are used for control and
expressions that represent the value to be returned or the action to be
taken. The latter expressions can, in general, be a sequence of
expressions separated by semi-colons. The expressions used for control
are restricted to being simple expressions that compute a single array
value in one step. However, any assignment expression or expression
sequence can be made into a simple expression by enclosing it in
parentheses. Thus, the while-expression version of the read loop can be
written as:

    File := open "myfile "r;
    WHILE (Record := readfile File) ~= ??eof DO
      <process record>
    ENDWHILE;
    close File;
    

Although the embedding of assignments is permitted in Nial, it is a
practice that should be used with restraint. Its overuse can make
programs difficult to read.

## Blocks

The block concept in Nial is borrowed directly from Algol 60 and its
descendants. It is essentially the same as that in Pascal except for the
treatment of new variables assigned within the block. The symbols that
delimit the block are { and }. A block can declare variables to be LOCAL
or NONLOCAL. Without explicit declaration that it is NONLOCAL, a
variable used as the target in an assignment is automatically treated as
a local variable.

    loaddefs "block 1
    average IS OPERATION A {sum A / tally A}
    { NONLOCAL X;
      X := count 25;
      Y := 5 5 reshape X;
      EACH average cols Y }
    11. 12. 13. 14. 15.
    
    X
    1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
    
 
    Y
    ?undefined identifier: Y <***>
    

The block above declares X as a nonlocal variable. This creates a global
variable X. The use of X after the block shows that the value assigned
in the block has been passed to the global variable. Y is declared as a
local implicitly by its use as the target in the second assignment. The
attempt to use Y outside the block results in an *?undefined identifier*:
fault. The value of the block, which is the value of the last expression
in the block, is computed using the global operation *average* defined
before the block. The actions in the above session were stored in a file
*block.ndf* and brought into the workspace using *loaddefs*.

A block can also hold a definition. The following block illustrates the
same example as the one above with the operation *average* made a local
definition.

    { NONLOCAL X;
      average IS OP A { sum A / tally A }
      X := count 25;
      Y := 5 5 reshape X;
      EACH average cols Y }
      
    11. 12. 13. 14. 15.
    

## Defining a Parameterized Operation

The operation *average* defined in the previous section illustrates the
use of an operation-form in a definition. An operation-form begins with
the keyword OPERATION (which may be abbreviated to OP), is followed by a
list of one or more formal parameter names and then is followed by a
block or an expres sion-sequence in parentheses.

The formal parameters are local to the operation form and are treated as
local variables that are initialized when the operation-form is used.
For example:

    compare IS OPERATION X Y {
      Maxs := EACH max X Y;
      Mins := EACH min X Y;
      >= Maxs and <= Mins }
      
    2 5 3 9 compare 6 4 5
    l
    
    compare [3 4 5 6, 4 5 6 7]
    o
    

The name *compare* is associated with the operation-form by the keyword
IS. The operation-form has two formal parameters X and Y, and its body
is the block with a sequence of three expressions. The parameters, X and
Y, and the variables, Maxs and Mins, are all local to the
operation-form. The two actions after the definition illustrate the use
of compare in infix and prefix style respectively.

When compare is used, the two arrays that are provided are called the
actual arguments. The evaluation of a use of *compare* is in two steps:

1. Assign the values of the actual arguments to the formal parameters.
2. Evaluate the body of the operation-form with the parameters initialized.

The result of the application is the value of the body, which is the
value of the last expression in the expression sequence in the body.

Nial's interpretation of parameter passing is equivalent to
call-by-value in Pascal. There is no equivalent in Nial to Pascal's
call-by-variable form, although it can be simulated using operations and
variable names.

An operation-form that has only one formal parameter assigns the entire
actual argument to the parameter. In Chapter 1, the operation *average* is
applied to a list of numbers which are assigned to the single formal
parameter A.

If *compare* is used in infix style, the actual argument is the pair
formed from the two surrounding expressions. In general, an
operation-form with two or more formal parameters splits the actual
argument, placing the items of the argument in the parameters in a left
to right order. In the latter case, the length of the parameter list and
the tally of the actual argument must agree.

An operation-form in Nial corresponds to a first-order lambda-form in
Lisp. As in Lisp, an operation-form can be written in line and used
without being given a name. For example:


    (OPERATION X Y { Maxs:=EACH max X Y;
        Mins := EACH min X Y;
        >= Maxs and <= Mins }) [3 4 5 6, 4 5 6 7]
        
     o

In such a use, the operation-form must be enclosed in parentheses.


## Defining a Transformer

A transformer is used to modify an operation or an atlas of operations
in a systematic way. Most of the general transformers needed for
programming in a functional style are built into Nial. Nial provides a
mechanism for defining additional transformers.

The mechanism for defining a transformer is called a transformer-form.
It consists of the keyword TRANSFORMER followed by a formal operation
parameter list followed by an operation-expression. TR is a short form
for the keyword.

A simple example of a transformer definition using a transformer-form is

    TWICE is TRANSFORMER f OPERATION A { f f A}
    TWICE (5+) 20
    30
    

The transformer modifies the operation parameter f by applying it twice
to the argument. In the example, the operation (5+) is applied twice to
20 to give 30.

A transformer can also be used to describe an algorithm for a class of
problems. Such an algorithm is called a schema. A typical use for this
purpose is given by the following example of a transformer to do a
general depth recursion on a labelled tree data structure, where at
each node the first item is the label and the rest of the items form a
list of nodes holding the "children". A leaf of the tree is a node with
a single item.

    RECUR is TR process_leaf process_label combine
    OP Tree {
      IF tally Tree = 1 THEN
        process_leaf first Tree
      ELSE
        New_label := process_label first Tree;
        Children := rest Tree;
        New_children := EACH (RECUR [process_leaf, process_label, combine]) Children;
        New_label combine New_children
      ENDIF }
      

    Tree gets ["Root,
                ["L10,
                  ["L20, 25 ,13 ],
                  ["L21, -20, 45, 15 ]],
                ["L11, 100],
                ["L12, 1000] ]
                
    +---------------------------------------------------+
    |Root|+---------------------------+|L11 100|L12 1000|
    |    ||L10|L20 25 13|L21 -20 45 15||       |        |
    |    |+---------------------------+|       |        |
    +---------------------------------------------------+
    

The schema does a depth-first recursion on the tree data structure
testing for a leaf. The operation parameter *process_leaf* is applied to
the leaf if one is found. Otherwise, the label of the node is processed
using *process_label*, the schema is applied recursively to each child of
the node and the two results are combined using operation parameter
*combine*. In a use of *RECUR*, the actual operation arguments are provided
as an atlas.

    leaves is RECUR [pass, pass, link sec ond]
    leaves Tree
    
    25 13 -20 45 15 100 1000
    

In leaves, operation *pass* is used for the first two arguments
indicating that no processing is done to the labels and leaves. The
composed operation *link second* is passed as the third argument and has
the effect of linking the result of applying the schema to the children.
The effect is to gather all the leaves of the tree into a list.

    sum_leaves IS RECUR[pass,pass,sum second]
    sum_leaves Tree
    1178
    

In *sum_leaves*, the effect is similar except the child nodes are
combined with the composed operation *sum second*. The effect is to form
the sum of the leaves.

    add_one IS RECUR [1+,pass,hitch]
    add_one Tree
    +---------------------------------------------------+
    |Root|+---------------------------+|L11 101|L12 1002|
    |    ||L10|L20 26 14|L21 -19 46 16||       |        |
    |    |+---------------------------+|       |        |
    +---------------------------------------------------+
    

In *add_one*, the effect is to form the array of the same structure as
Tree with one added to each of the leaves. This is achieved by using
(1+) to process the leaves, using pass on the labels and using *hitch* to
rebuild the tree at each level.

    change_labels IS RECUR [pass,?LAB first,hitch]
    change_labels Tree
    
    +--------------------------------------------------+
    |LAB|+---------------------------+|LAB 100|LAB 1000|
    |   ||LAB|LAB 25 13|LAB -20 45 15||       |        |
    |   |+---------------------------+|       |        |
    +--------------------------------------------------+
    

In *change_labels* the effect is to form the array of the same structure
as Tree with each label replace with the phrase LAB. This is achieved by
using *pass* to process the leaves, using the curried operation *LAB*
first on the labels and using hitch to rebuild the tree at each level.
