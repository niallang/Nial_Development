---
title: "Getting Started - Nial Introduction, Chapter 1"
permalink: /ndocs/intro/chapter1/
layout: single
---


# Getting Started
### Chapter 1

- [A First Session](#a-first-session)
- [Some Differences with Other Languages](#some-differences-with-other-languages)
- [Defining Operations](#defining-operations)

Conventional high level languages, such as Fortran, C or Pascal, are
designed for the creation of complete programs using a text editor. The
program is translated (compiled) to machine language and finally
executed (run) to carry out its task. Conventional program development
consists of an edit, compile, and run cycle that is either managed
manually or controlled through a programming environment.

Q'Nial is designed for the creation of an evolving set of program
fragments developed interactively. Q'Nial comes as a console
version where a session is started by typing

    nial -i

Q'Nial responds by
presenting a banner of information and then awaits input at the
keyboard. You interact with Q'Nial by typing a program fragment on the
keyboard and reviewing the result displayed on the screen. Each
keyboard entry is completed by pressing *Enter*. To end the session,
you type *Bye* or *Continue*.


## A First Session


The following session uses the Linux Console Version of Q'Nial:


    > nial -i
    Q'Nial V7.0 Open Source Edition Intel x86 64bit Linux Jul 29 2015
    Copyright (c) NIAL Systems Limited
    clear workspace created    
    
         A := 'hello'
    hello

         B := 'world';

         A link B
    helloworld

         link A ' ' B
    hello world

         Bye



The messages indicate that the session starts with the initial
workspace, which provides access to predefined Nial objects.

The session begins with an assignment of the string 'hello' to the
variable A. A is the variable; *gets* is the assign action; the string
'hello' ( a list of characters) is the expression. The string is
displayed as the output of the interaction. The entry typed at the
keyboard is shown indented because the default prompt for Q'Nial, the
indication that Q'Nial is awaiting input, is five blank spaces.

    
         A gets 'hello'
    
    hello


The default behaviour is that every input action in Q'Nial returns a
value and that the value is displayed immediately after the input. The
value of an assignment action is the value of the expression to the
right of *gets*. In the example above, the expression is the string
'hello'.

    
        B gets 'world' ;
    

An input action that ends with a semicolon ( ; ), however, does not
display a value. This occurs because the value returned in this case is
a special value *?noexpr* which, by convention, is not displayed in the
top level loop.

    
         A link B
    
    helloworld
    

The next input is an infix use of the predefined Nial operation *link*
which joins strings together. The result is the joined string with no
separation between the strings held by A and B.

    
         link A ' ' B
    
    hello world
    

The above input uses *link* in a prefix manner to join three strings: the
string held by A, a string holding a single blank character and the
string held by B. The result is a string with the words separated by a
blank.

    
         Bye
    

The final input action, *Bye*, ends the session. If *Continue* were used
instead of *Bye*, the workspace information would be saved as the file
*continue.nws* in the current directory and the next invocation of Q'Nial
from within the same directory would restart the session with variables
A and B defined.

## Some Differences with Other Languages


The above session illustrates some differences between Nial and
conventional compiled languages. First, the use of a console version of
Q'Nial is driven by a top level loop which consists of:


- Issue the prompt.
- Read an input action typed after the prompt.
- Execute the action.
- Display the value of the action if it is not the fault *?noexpr*.


This is similar to the edit-compile-run cycle of compiled languages but
differs in that the edit-compile-run cycle is done on a program fragment
consisting of one line and the output is automatically displayed. This
paradigm is common in other high level languages that have an
interactive interface such as Lisp, Prolog, APL and Smalltalk.

A second difference is the lack of declarations for the variables A and
B. Nial is an untyped language in the sense that variables and operation
parameters do not have a fixed object type associated with them. In this
case, both A and B are assigned objects that are strings. The type
information is associated with the objects rather than with the
variables.

A third difference is illustrated by the operation *link*. In Nial, all
data objects have both structure and content. Operations, which
correspond to value-return ing functions in compiled languages, map data
objects to data objects.

Nial can use display settings to provide more meaning ful displays of
values. The examples in "A First Session" were shown in sketch, nodecor
modes. The following session shows a similar set of examples in diagram,
decor modes. In the examples, the double quote mark (") before diagram
is a phrase designator indicating to Q'Nial that diagram is a word or
phrase, which is data, and not a variable name.



         set "diagram
    sketch

         set "decor
    "nodecor

    
         A gets 'Hi'
         
    +-----+
    |`H|`i|
    +-----+

         B gets 'Mom'
    +--------+
    |`M|`o|`m|
    +--------+


          C gets A ' ' B
    +-----------------------+
    |+-----+|+--+|+--------+|
    ||`H|`i|||` |||`M|`o|`m||
    |+-----+|+--+|+--------+|
    +-----------------------+


         link C
    +-----------------+
    |`H|`i|` |`M|`o|`m|
    +-----------------+


The operation *set* is used to control a number of internal switches. Its
argument is a phrase or string indicating the setting desired. Here it
is used twice to place Q'Nial into the diagram-decor mode of output
display. In this mode, the full structure of data objects is displayed
(diagram mode) and the contents are decorated to distinguish among
atomic types (decor mode). With the display modes set, it can be seen
that the argument to *link*, stored here in variable C, is a triple (a
list of length three) of lists of characters of lengths 2, 1 and 3
respectively. The diagrams illustrate that *link* joins the lists that are
items in its argument to form a list of characters of length 6.

The next example illustrates that data objects such as C can be built up
by combining other data objects without any detailed description of
their structure. The data objects are dynamic in that their size and
struc ture do not have to be described before they are computed.

Returning to the discussion of *link*, the first example used it as an
infix operation on A and B and the second as a prefix operation on the
triple assigned to C. The dual usage is possible because Nial views all
operations as unary operations. That is, they take a single array as an
argument and return a single array as a result. Nial translates an infix
use to a prefix use applied to the pair formed from the two arguments.
Thus, the action A *link* B is equivalent to *link* A B. As well, *link* can
be used to join lists of lists of any type or even of mixed type.

The work done by *link* in forming its result involves looping over the
items of the argument and, for each item, looping over its items to move
them to the result. Thus, for a task of joining m lists each of length
n, *link* does m times n elementary data move ments. Most of the
predefined operations of Nial have the property of doing a substantial
computation in terms of the elementary steps done by a conventional
compiled language. This higher level view of data and operations on data
gives Nial an expressive power that supports rapid problem solving.

## Defining Operations

The actions presented in the two previous sessions use built-in
capabilities of Nial. In order to program, new functional objects that
capture a number of computa tional steps under one name must be created.
This process is called abstraction and is central to all programming.
Consider the following:

    
         Numbers gets 45.2 3.7 -35.3 87.14
    +--------------------+
    |45.2|3.7|-35.3|87.14|
    +--------------------+
    
         sum Numbers
    100.74
    
         tally Numbers
    4
    
         sum Numbers / tally Numbers
    25.185
    

The four actions above assign a list of numbers to the variable Numbers,
sums the list of numbers, measures the length of the list of numbers and
computes the average of the list of numbers. These actions compute array
values and are called array-expressions.

         average IS OPERATION A {sum A / tally A}
         average Numbers
    23.788

         see "average
    average IS OPERATION A {
    sum A / tally A }

The next action is a definition of a new operation that computes the
average of a list of numbers. An action that is a definition does not
return a value. The definition consists of a name for the object being
defined *average*, the reserved word IS followed by reserved word
OPERATION followed by an identifier A as its one formal parame ter.

The body of the function is a block that has just one expression, namely
the expression that computes the average of A. The action following the
definition uses *average* to compute the average of the list held by
Numbers. The final action uses the operation *see* to display the
definition of average in canonical form. When a definition is processed
it is stored in the workspace for later use. It can be viewed by see
(and edited using defedit in console versions of Q'Nial).

The session just described shows that a short definition can be typed
as an action in the top level loop. However, this technique is
impractical for operation definitions that span several lines. To
prepare longer definitions, it is preferable to use a text editor and
then to load the definitions as one action. The operation *loaddefs*
provides the capability to load one or more definitions that are stored
in a text file.

There are several ways to prepare a longer definition. The simplest way
on a multiwindow system is to invoke a text editor in a separate window,
create the definition and store it as a file, say *mydef.ndf*. A second
way is to open the default system editor using the action *edit "mydef.ndf*, 
create the definition and store it. In either case, the
action *loaddefs "mydef* is used to bring the definition into the
workspace. A definition file name is required to have the extension
*.ndf*, but the extension may be omitted in the argument to *loaddefs*.

Definitions created as actions or by using an editor can be retained by
using the operation save to store the workspace as a binary file. The
workspace can be reactivated later using *load*.
