# Crafting Interpreters

This repo consists of an interpreter and a compiler for a language called 'Lox', they were made following the book called 'Crafting Interpreters' which can be found at: https://craftinginterpreters.com

The first interpreter is written in Java (JLox) and is an abstract syntax tree (AST) interpreter which serves more as a way to learn some intricacies of how languages work and how they are made. Being an AST and written in Java and running on the JVM it's slow and unoptimized.

The second is a compiler written in C (CLox), it's a bytecode compiler and is the 'final product', a fully functional and fast compiler written for this little scripting language.
