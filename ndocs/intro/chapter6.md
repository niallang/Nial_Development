---
title: Host System Interface - Nial Introduction, Chapter 6
layout: single
classes: wide
---


# Host System Interface

- [Use of Files](#use-of-files)
- [Interpreter Mechanisms](#interpreter-mechanisms)

Q'Nial is designed as a portable language processor that runs in a very
similar way on different operating systems. This design goal leads to an
abstract ap proach to accessing system functions. A second design goal
is to make many of the internal capabilities of the interpreter
accessible through operations which permit the user to exploit the
underlying mechanisms. Some of the latter operations are dependent on
the specific implementation details.

## Use of Files

In Q'Nial there are four ways that files can be used:

1.    conventional sequential files accessed by line
2.    direct access files accessed by byte position,
3.    Nial direct access files where components are Nial strings,
4.    Nial direct access files where components are arbitrary arrays.

The first method can be used to access and create text files that are
used by other applications. The actions on the file equate a Nial string
with a record of text in the file. The end-of-line indication is
stripped off on input and restored on output, allowing Nial programs to
work compatibly across different host systems. Q'Nial supports
input/output operations to a console or terminal as accessing the
sequential system files *stdin* and *stdout*. This means that *readscreen* is
implemented by writing the argument to *stdout* and then doing a read of
a record of data from *stdin*.

The second method is used for direct manipulation of a file using the
standard Posix systems functions. A file is viewed as a sequence of
bytes and two operations *readfield* and *writefield* are provided to read
bytes from a file or write bytes to one.

The Nial direct access methods use two host system binary files to
represent a conceptual file, one to hold the index and the other to hold
the data records. An example of the use of direct files can be seen in
the library file *simpledb.ndf*.

The console versions of Q'Nial also supports a window capability for
character-based windows. This is integrated with the normal sequential
keyboard input, screen output so that programs that use simple input and
output can work with or without windows. An extensive editor capability
is provided with the windowing capability.

## Interpreter Mechanisms

The internal features of the Q'Nial interpreter made available to the
user include:

| Name        | Function                                               |
| ----------- | ------------------------------------------------------ |
| scan        | scan a string of program text into a token stream |
| parse       | parse a token stream into a parse tree. |
| eval        | evaluate a parse tree |
| execute     | evaluate a string of program text |
| descan      | convert a token stream to a list of strings  |
| deparse     | convert a parse tree to an annotated token stream |
| picture     | create data pictures as character tables |
| paste.      | paste a collection of character tables together |
| positions   | give coordinates of the position of items in a picture |
| assign      | assign a value to a variable name or cast |
| value       | look up the data value associated with a variable |
| apply       | apply an operation to a value the operation being given by its name or as a parse tree |
| update      | do an indexed assignment |


These capabilities permit an application to manipulate a program as
data and to control its execution. For example, in a rule-based
artificial intelligence system, the rules can be stored as text while
the system is being designed. The operation *execute* can be used to
evaluate the rules during this stage. Later, when the application has
stabilized, a more efficient version can be built using *parse scan* to
process the rules into parse trees and using *eval* to evaluate them. The
equation:

    execute Str = eval parse scan Str
    

which holds for every string that denotes a Nial array expression,
drives this approach.

The operation *apply* can be used in combination with a selection process
to permit dynamic application of one operation from a set of operations.
This can be used to simulate method application in a object-oriented
system.

The operations associated with the *picture* operation allow the user to
build output screens and reports by pasting together component parts
using the operation *paste*.
