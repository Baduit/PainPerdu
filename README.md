# Pain Perdu
## Description
This is an esoteric language I created for fun.

Here a little hello world example:
```C
+72] +29] +7]] +3] -79] +55] +24] +3] -6] -8] -67] -23]
```

This repository contains the specification (in this readme) and the official implementation.

The default extension for the source file is "__.pain__".

## Specification
### General
The __machine__ is the program interpreting/executing/compiling the _PainPerdu_ code.

The __stack__ is the memory of the machine, it grows automatically. The maximum size is implementation defined.
> Official implementation will use the maximum size of an std::vector

The stack is made of __cases__, the size of a cases is an unsigned integer value. Overflow of the value reset the value to 0 and underflow set the value to 255.

The __cursor__ is the pointer in the stack.

An __instruction__ is an action to perform.

An __annotation__ is an information about the code, annotations are evaluated before 1st instruction is executed.

The __recipe__ is the list of instruction of the machine.

The __step__ represents the actual position of the machine in the recipe.

The machine executes the instruction at the step then advance the step until it reachs the end of the recipe. Some instructions may change the position of the step.

An __indentifier__ is a list of alphanumeric or underscore characters representing the name of a label or a reference. Identifiers beginning with 2 underscores are reserved. 

A __label__ is a reference to a position in the recipe, it can be created an annotation. If you try to use a label but the label does not exist, the machine must stop.

A __reference__ is a location to a case, it can be created with an instruction. If you try to use a reference but the reference is not defined the machine must stop.

> The same identifier can be used for a reference and a label.

When the machine ends it will return the value of the last modified case.

It is possible to add space, tabulations or new line between the instructions or annotations.

### Comments
Comments are all the characters between { }.

There is no way to escape these characters, anyway there is no other context where the characters '{' and '}' are valid.

### Annotations
In this table:
* __id__ represents an identifier.

| Symbols        | action 
| -------------- | -------------
| :id            | define a label id pointed to the step next step, it cannot override an existing label, if there is not instruction after the annotation, it points to the end of the program and moving the step to this label cause the end of the program


### Instructions
In this table 
* __n__ represents a positive integer.
* __id__ represents an identifier.

| Symbols        | action 
| -------------- | -------------
| >n             | move the cursor of n cases to the right
| >id            | move the cursor of n cases to the right where n is value at the case pointed the reference id
| <n             | move the cursor of n cases to the left
| <id            | move the cursor of n cases to the left where n is value at the case pointed the reference id
| +n             | increment the value pointed by the cursor of n (n must be between 0 and 255 included)
| +id            | increment the value pointed by the cursor of n where n is value at the case pointed the reference id
| -n             | decrement the value pointed by the cursor of n (n must be between 0 and 255 included)
| -id            | decrement the value pointed by the cursor of n where n is value at the case pointed the reference id
| ;              | reset the value at the current location of the cursor to 0
| #id            | define of a reference id to the current location of the cursor, it can override an existing reference
| .id            | undefine a reference id, the machine crash if the reference does not exist
| @id            | move the cursor to the reference id, the machine crash if the reference does not exist
| *id            | move the step to the label id, the next instruction executed will be the instruction pointed by the label, the machine crash if the label does not exist
| &id            | move the step at the location the step was the last time *id was called the machine crash if the label does not exist or if *id was never called
| ?              | the next instruction is executed only if the case at the cursor is not equal to 0
| ?n             | the next instruction is executed only if the case at the cursor by is equal to n (n must be between 0 and 255 included)
| ?id            | the next instruction is executed only if the case at the cursor by is equal to n where n is value at the case pointed the reference id
| !id            | the next instruction is executed if the cursor is not at the reference id, the machine crash if the label does not exist
| $id            | the next instruction is executed if the reference id exists
| [              | read 1 byte from the standard input and set the case at the cursor to the read value
| ]              | write the case at the cursor in the standard output

### System references
| identifier          | description
| ------------------- | ---------------------------------------------
| \_\_begin__         | 1st case of the machine
| \_\_end__           | last case already accessed by the machine
| \_\_last_modified__ | last case modified by the machine
| \_\_here__        | reference at the current position of the cursor

### System labels
| identifier          | description
| ------------------- | ---------------------------------------------
| \_\_start__         | beginning of the machine (if the step is here, the next instruction executed will be 1st instruction of the recipe)
| \_\_exit__          | end of the machine (if the step is here, the machine stop)

## Example
A simple hello world example
```C
+72     ]
>1 +101 ]
>1 +108 ]]
>1 +111 ]
>1 +32  ]
>1 +87  ]
>1 +111 ]
>1 +114 ]
>1 +108 ]
>1 +100 ]
>1 +33  ]
>1 +10  ]
```

A more advance example that print hello world 10 times:
```
+72     
>1 +101 
>1 +108 
>1 +108 
>1 +111 
>1 +32  
>1 +87  
>1 +111 
>1 +114 
>1 +108 
>1 +100 
>1 +33  
>1 +10 
>1 #this_is_to_add_a_backslash_zero

>1 #nb_iteration +10
:main_loop
@__begin__ *print
@nb_iteration -1
?*main_loop

{ Add a little safety so this code is not called implicitly}
*print_skip
:print
:print_impl { If we are not at a \0 it we go to putchar } ?*print_putchar { Else go to the end } *print_end
:print_putchar { Print the character then move in the stack to the right then go back at print_impl} ]>1 *print_impl
:print_end { Rewind where print was called } &print
:print_skip
```

## Try it online!
You can try it online [here](https://painperdu.baduit.eu/) with the online editor. 

You can note that there is a difference between the online version and the local version: the instruction '[' to read a character is not implemented.

But there is an exclusive  feature! There is a canvas. The 1st pixel uses the value of 1st case of the stack for the color red, the 2nd for its color green and the 3rd for the blue and so on.

## Interpreter
### Build instructions
It requires a recent version of cmake and a compiler supporting C++20.

All dependencies are either directly included in the repo or included as a submodule.

It uses:
- [Rang](https://github.com/Baduit/PainPerdu/tree/master/lib/external/Rang) a simple library to have color for my (very basics) logs
- [Brigand](https://github.com/edouarda/brigand) a metaprogrammation library
- [Crepuscule](https://github.com/Baduit/Crepuscule) my own tokenizer library
- [MagicEnum](https://github.com/Neargye/magic_enum) cool library allowing to have reflection with enums
- [nameof](https://github.com/Neargye/nameof) usefull library
- 


The web version also uses:
- [nlohmann json] (https://github.com/nlohmann/json) do I really need to present it ?
- [cpp-httplib](https://github.com/yhirose/cpp-httplib) a very simple http library in 1 header or 1 header and 1 source file.
- [ut](https://github.com/boost-ext/ut) a single header macro free unit test framework

### Usage
The name of the interpreter is 'ppb' (Pain Perdu Baker).

`./pbb` will enable the interactive mode, it will interpret what you write on stdin

`./ppb filename` will read the file and interpret it.

## Why "PainPerdu" ?
I like to name stuff with a good bakery name. The Pain Perdu (also known as 'French toast' or 'Eggy Bread') is a very old, easy to do and very good bakery. Also, it exists (sometimes with slight variations) in a lot of country.
