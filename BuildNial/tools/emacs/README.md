nial-mode
=========

An Emacs major mode for working with Nial (Q'Nial) contributed by *tangentstorm*. This language mode provides syntax highliting for
Nial code and uses the Emacs *Command Interpreter*
(comint) to run a Nial interpreter. 

It supports the sending of text from a Nial language mode buffer to the Nial Interpreter.

Transfer the three Emacs files to a directory of your choosing (I use the subdirectory of my home directory
*lib/nial*) and add the following to your *~/.emacs* file.

    (add-to-list 'load-path "~/lib/nial/")
    (require 'ob-nial)
    
The extension defines a number of Emacs commands

- *nial-console* ensures a Nial console exists. If needed it will create a comint buffer and initiate execution of Nial
- *nial-console-execute-region* sends the selected region of text to the Nial console for execution by Nial
- *nial-console-execute-line* sends the current line to the Nial console for execution
- *nial-console-execute-buffer* send the current buffer to the Nial console for execution

You can bind these commands to the key-strokes of your choice.


