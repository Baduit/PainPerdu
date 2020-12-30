# Pain Perdu
## Description
This is a stack based esoteric langage I create for fun.

This repository contains the specification (in this readme) and the official implementation.

## Specification
### General
The __stack__ is the memory of the program, it grows automatically. The maximum size is implementation defined.
> Official implementation will use the maximum size of an std::vector

The stack is made of __cases__, the size of a cases is an unsigned integer value. Overflow of the value reset the value to 0 and underflow set the value to 255.

The __cursor__ is the pointer in the stack.

An __instruction__ is an action to perform.

The __recipe__ is the list of instruction of the program.

The __step__ represents the actual position of the program in the recipe.

The program execute the instruction at the step then advance the step until it reachs the end of the recipe. Some instructions may change the position of the step. Example:
- program starts with the step à the beginning of the recipe
- program executes 1st instruction
- program advances the step
- program executes 2nd instruction
- program advances the step
- program executes 3rd instruction that changes the position of the step to 5th instruction
- program advances the step (even if we jumped to the 5th instruction, the 5th instruction os not executed because the program will advances the step just after each instruction)
- program executes 6th instruction
- program exits because the recipe is over

An __indentifier__ is a list of alphanumeric or underscore characters representing the name of a label or a reference. Identifiers beginning with 2 underscores are reserved. 

a __label__ is a reference to a position in the recipe.

a __reference__ is a location to a case.

> The same identifier can be used for a reference and a label

When the program ends it will return the value of the last modified case.

### Instructions
In this table :
* __n__ represents a positive integer
* __id__ represents an identifier

| Symbols        | action 
| -------------- | -------------
| >n             | move the cursor of n cases to the right
| <n             | move the cursor of n cases to the left
| +n             | increment the value pointed by the cursor of n
| -n             | decrement the value pointed by the cursor of n
| #id            | define of a reference id to the current location of the cursor, it can override an existing reference
| @id            | move the cursor to the reference id
| :id            | define a label id at the step, it can override an existing label
| *id            | move the step to the label id
| ?              | the next instruction is executed only if the case at the cursor is not equal to 0
| ?n             | the next instruction is executed only if the case at the cursor by is equal to n
| !id            | the next instruction is executed if the cursor is not at the reference id
| [              | read 1 byte from the standard input and set the case at the cursor to the read value
| ]              | write the case at the cursor in the standard output
| $id            | the next instruction is executed if the reference id exists
| .id            | the next instruction is executed if the label id exists

### System references
| identifier          | description
| ------------------- | ---------------------------------------------
| \_\_begin__         | 1st case of the program
| \_\_end__           | last case already accessed by the program
| \_\_last_modified__ | last case modified by the program

### System labels
| identifier          | description
| ------------------- | ---------------------------------------------
| \_\_start__         | beginning of the program (if the step is here, the next instruction executed will be 1st instruction of the recipe)
| \_\_exit__          | end of the program (if the step is here, the program stop)
| \_\_last_modified__ | last case modified by the program

## Try it online

## Interpreter
### Build instructions
### Usage

## Why "PainPerdu" ?
I like to name stuff with a good bakery name. The Pain Perdu (also known as 'French toast' or 'Eggy Bread') is a very old, easy to do and very good bakery. Also it exists (sometimes with slight variations) in a lot of country.
