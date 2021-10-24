---
title: An Overview of Nial - Nial Introduction, Chapter 2
---


# An Overview of Nial
### Chapter 2

- [The Data Objects](#the-data-objects)

Nial is based on a mathematical treatment of array data structures,
informally called array theory, and on standard features borrowed from
other programming languages. Thus, it combines a mathematical approach
to the treatment of data with well understood programming concepts.
These capabilities are presented in an interactive environment driven by
the Q'Nial interpreter. Nial is ideal for exploratory or experi mental
computing, but can also be used effectively for doing intensive
computations involving large data sets.

The concept of program development in a conventional compiled language
does not apply to Nial. In Nial, the unit of translation is an action
which either defines a new functional object to be held in the workspace
or describes a data object to be computed in the form of an array
expression. The latter may assign the data object to a variable
retaining the data object in the workspace. A session consists of a
sequence of interactions with the pro grammer or user in which actions
are entered. As the session proceeds, definitions and variables are
collected in the active workspace. The meaning of an action is
determined by definitions and vari ables already processed.

The operation *loaddefs* can be used as an action that processes several
other actions stored in a file. A program in the conventional sense can
be created as one large definition in a file and then loaded and invoked
using *loaddefs*; but such an approach ignores the advantages of being
able to work in a combination of textual definition and direct
interaction. Loaddefs may be used to bring in a single definition or a
set of definitions prepared using an editor and can call itself
recursively.

## The Data Objects

All data objects in Nial are considered to be array data structures. An
array is a collection of data objects organized along axes. The most
frequently used structure is the list which has only one axis. A string
is a list of characters.

Any collection of data objects can be stored in a list; there is no
requirement that all the items have the same type. Thus, record types in
Pascal or structs in C can be represented in Nial as lists.

Consider the following examples of lists displayed in diagram mode. A
list of characters:

         'hello world'
        
    +---------------------+
    |h|e|l|l|o| |w|o|r|l|d|
    +---------------------+
    

A list of numbers created by the operation count which returns the list
of integers starting at 1:

         count 12
         
    +--------------------------+
    |1|2|3|4|5|6|7|8|9|10|11|12|
    +--------------------------+
    

A list of two items of different type:

         "age 23
         
    +------+
    |age|23|
    +------+
    

A list of two items, each of which is a list of items:

         (count 5) 'hello'
         
    +-----------------------+
    |+---------+|+---------+|
    ||1|2|3|4|5|||h|e|l|l|o||
    |+---------+|+---------+|
    +-----------------------+
    

Arrays can nest within other arrays. The above example is easier to
comprehend when the boxes of the innermost lists are omitted as in the
example below:

         set "sketch; (count 5) 'hello'
         
    +---------------+
    |1 2 3 4 5|hello|
    +---------------+
    

An array with two dimensions is called a table. Tables are convenient
data structures in which to store character information that is to be
displayed on a computer screen; or numeric data that represents a matrix
in a scientific computation:

         set "diagram; T gets 3 4 reshape count 12
         
    +----------+
    |1| 2| 3| 4|
    +-+--+--+--|
    |5| 6| 7| 8|
    +-+--+--+--|
    |9|10|11|12|
    +----------+
    

         set "sketch; T
         
    1  2  3  4
    5  6  7  8
    9 10 11 12
    

A table is built using the operation *reshape* with first argument giving
the number of rows and columns and the second the list of items. T is a
3 by 4 table. In diagram mode, a table is boxed. In sketch mode, if all
the items are atomic, the boxing is omitted. An array having all atomic
items is said to be simple.

There are many operations that manipulate tables in Nial.

         transpose T
         
    1 5  9
    2 6 10
    3 7 11
    4 8 12
    
   
         rows T
         
    +--------------------------+
    |1 2 3 4|5 6 7 8|9 10 11 12|
    +--------------------------+
    
   
         cols T
         
    +--------------------------+
    |1 5 9|2 6 10|3 7 11|4 8 12|
    +--------------------------+
    

The rows of T and the columns of T are related by the operation *pack*,
which interchanges the levels of a list of lists. The operation *mix* can
be used to convert a list of lists to a table:

         pack rows T
         
    +--------------------------+
    |1 5 9|2 6 10|3 7 11|4 8 12|
    +--------------------------+
    

         mix rows T
         
    1  2  3  4
    5  6  7  8
    9 10 11 12
    

         mix cols T
         
    1 5  9
    2 6 10
    3 7 11
    4 8 12
    

Higher dimensional arrays are sometimes used in three dimensional
modelling. A three dimensional array can be created using *reshape*:

         A gets 2 3 4 reshape count 24
         
    1  2  3  4   13 14 15 16
    5  6  7  8   17 18 19 20
    9 10 11 12   21 22 23 24
    

The array A is a 2 by 3 by 4 array of the first 24 integers counting
from one.

The operation *raise* can be used to partition a higher dimensional array
into an array of lower dimensional ones:

         List_of_tables gets 1 raise A
    
    +----------------------+
    |1  2  3  4|13 14 15 16|
    |5  6  7  8|17 18 19 20|
    |9 10 11 12|21 22 23 24|
    +----------------------+
    

         Table_of_lists gets 2 raise A
    
    +-----------------------------------+
    |1  2   3  4| 5  6  7  8|9 10 11 12 |
    +-----------+-----------+-----------|
    |13 14 15 16|17 18 19 20|21 22 23 24|
    +-----------------------------------+
    

List_of_tables is a list of two 3 by 4 tables. Table_of_lists is a
2 by 3 table of lists of length four. In the following example, the
operation *reverse* is used directly on List_of_tables giving
Rev_tables. The items of List_of_tables can be reversed by modifying
the operation *reverse* using *EACH*. In these examples, *gets* is replaced by
the symbol *:=* which is an alternative notation for assignment.

         Rev_tables := reverse List_of_tables
         
     +----------------------+
     |13 14 15 16|1  2  3  4|
     |17 18 19 20|5  6  7  8|
     |21 22 23 24|9 10 11 12|
     +----------------------+
     


         Rev_tables_items := EACH reverse List_of_tables
         
    +----------------------+
    |12 11 10 9|24 23 22 21|
    | 8  7  6 5|20 19 18 17|
    | 4  3  2 1|16 15 14 13|
    +----------------------+
    

The effect of a *raise* can be undone using *mix*:

         mix List_of_tables
         
    1  2  3  4   13 14 15 16
    5  6  7  8   17 18 19 20
    9 10 11 12   21 22 23 24
    

For mathematical completeness, atomic data objects (also called atomic
arrays) such as booleans, integers, characters and real numbers are
treated as arrays with no axes and are viewed as self containing. The
effect of the latter choice is that the operation *first* used on an atom
returns the atom.

Nial has two additional atomic types not found in conventional
languages: phrases and faults. A phrase is an atomic unit of literal
data. Every phrase cor responds to a string but the internal structure
is concealed so that the character data cannot be directly accessed. The
string content can be retrieved using the conversion operation *string*.
Q'Nial stores phrases uniquely, that is, only one copy of each phrase in
use is kept in the workspace. This choice allows a phrase to be
represented as an integer that points to the content internally. The use
of a unique representation results in efficient equality comparisons
and copying of phrase data.

Faults are similar to phrases but are used to denote special values or
error conditions. Faults that report errors cause an interrupt of the
current execution when they are created unless the default behaviour of
Q'Nial for fault triggering has been modified. The special value faults
such as *?noexpr* and *?eof* behave like phrases except that they can be
tested using the test *isfault*.

As an illustration of the use of Nial data objects and some of the data
manipulation capabilities in Nial, consider the requirement to display a
table of data with appropriate labels on the rows and columns and an
overall label. Given the data table:

         Data
         
    23 34 12
    38 18 26
    37 13 42
    

produce the following display as a table for a report:

    Crop Yields Oats Rice Corn
    None          23   34   12
    Arsenic       38   18   26
    Old Lace      37   13   42
    

The operation *labeltable*, defined in the Nial library, can be used for
this purpose.

         library "labeltab; see "labeltable
         
    labeltable IS OPERATION Corner Rowlabel Columnlabel Table {
      % Combine the labels ;
      Firstrow := Corner hitch Columnlabel ;
      % Hitch labels to the rows ;
      Labelledrows := Rowlabel EACHBOTH hitch rows Table ;
      % Hitch labels to the rows and mix ;
      mix ( Firstrow hitch Labelledrows ) }
    

First the data objects required for applying the opera tion are
prepared:

         Data := 3 3 reshape 23 34 12 38 18 26 37 13 42 ;
         Corner := phrase 'Crop Yields' ;
         Collab := "Oats "Rice "Corn ;
         Rowlab := EACH phrase 'None' 'Ar senic' 'Old Lace' ;
         

To do this, the table data is entered as a list and reshaped. The label
information is specified as phrases, one for each row and column of the
data and one for the corner. The operation *phrase* is used to create a
phrase with embedded blanks for the corner. The transformer *EACH* is used
with *phrase* to build the row labels that also include blanks. Then the
operation *labeltable* is applied to the list of arguments yielding the
desired result:

        labeltable Corner Rowlab Collab Data
        
    Crop Yields Oats Rice Corn
    None          23   34   12
    Arsenic       38   18   26
    Old Lace      37   13   42
    

*Labeltable* is defined as an operation form, consisting of a header

         OPERATION Corner Rowlabels Column labels Table

and a body which is a block marked off by { and }. The block is a
sequence of six expressions separated by semicolons. The expressions
beginning with % are comments. The operation is called by the action:

         labeltable Corner Rowlab Collab Data

The action is evaluated in two steps. First, the actual argument
provided in the call (in this case, a list of 4 arrays) is assigned to
the formal parameters in the header. Then, the block forming the body of
the operation form is evaluated. This second step involves executing the
expressions in the block in order. The result of the execution of the
operation is the value of the last expression.

The details of the execution of the block are as follows. The
expression

         Firstrow := Corner hitch Columnlabels
         

attaches the value in *Corner* to the list in *Collab* to give the local
variable *Firstrow* the value

    Crop Yields Oats Rice Corn
    

The expression

         Labelledrows := Rowlabels EACHBOTH hitch rows Table ;

uses *hitch* modified by *EACHBOTH* to place the row labels on the
corresponding rows of the data table to give the local variable
*Labelledrows* the value

    +------------------------------------------------+
    |None 23 34 12|Arsenic 38 18 26|Old Lace 37 13 42|
    +------------------------------------------------+
    

The subexpression

         Firstrow hitch Labelledrows
    +---------------------------------------------------------------------------+
    |Crop Yields Oats Rice Corn|None 23 34 12|Arsenic 38 18 26|Old Lace 37 13 42|
    +---------------------------------------------------------------------------+
    

extends the list of labelled rows by placing the first row on the front.

The final expression

         mix ( Firstrow hitch Labelledrows )
         

uses operation *mix* to convert the list of lists holding the rows of the
result to the desired table:

    Crop Yields Oats Rice Corn
    None          23   34   12
    Arsenic       38   18   26
    Old Lace      37   13   42
    

Notice that there is no programming required to get the output displayed
in a tidy rectangular arrangement. The array display mechanism
automatically lines up columns using left justification for textual
fields and right justification for numeric fields.

The example illustrates the use of data manipulation operations. The
local variables (local by default) *Firstrow* and *Labelledrows* hold
inter mediate values that are used only once in the subsequent lines.
They serve as useful names for intermediate results and make the
operation more easily understood. An equivalent operation could be
defined as

    lab IS OP Co Rl Cl T { mix ((Co hitch Cl) hitch (Rl EACHBOTH hitch rows T)) }

However, such a cryptic definition is more difficult to understand since
the use of abbreviated names for the parameters and the defined
operation make the purpose of the definition obscure, and the long
single expression requires careful analysis to comprehend.

The illustration of how to display a table of data demonstrates the two
major programming components of Nial: data objects are manipulated by an
expres sion sublanguage that uses many predefined high level functions,
and expressions in the sublanguage are combined using control mechanisms
such as assignment, sequencing and function definition to provide a high
level mechanism for rapid programming.
