# README


[(online doc)](http://jleahred.github.io/jle_cpp_tk.doc/index.html)



## Modifs

...soon





## About
### Why

C++ is not the safest programing language in the world. In fact, it's easy to produce *undefined behaviour* working with C++.
It would be interesting to work with C++ in a safe way. Not only for me, also for less C++ experience people in a team.

Some years ago I wrote a C++ toolkit with regular expressions, tuples, class definition IDL, fsm IDL, config files. LL(n) parser, signal-slot, monotonic date time with adjustments...

These last years, C++'s world has moved a lot.

Now gcc and other compilers run a lot of features from C++11, C++14 and partially from C++17

Some of the tools I wrote or added to my library, are not necessary any more.
The implementation and API can also be improved.

For example... I wrote a smart_pointer called CountPtr. I also wrote to manage tuples,
I wrote a beautifull, platform independent, safe, and easy to use classes to deal with date_time and durations,
I added a small library for regular expressions. These features have been added to C++

Now, we also have type inference, lambdas, variadic templates, initializer lists...

It's time to enjoy and write a new set of tools based on modern C++ (more reduced than previous one, but with a better language)

Previously I wrote the documentation with **asciidoc**, muse over emacs and doxys. Now I'm going to do it with doxygen (I'm not a fan of these kind of tools, but it's a standard).


### Directives

My aim is different from C++.

- Safety
    - I don't like undefined behaviour
    - I don't like crashes due to programming mistakes
    - C++ is a dangerous language. It's easy to have crashes, I want a library protecting and helping the programmer.
- KISS
    - Easy to use API (the implmentation could be complex)
- Avoid write only code using this library. The code using this library...
    - could not be simple to write
    - has to be easy to read
- YAGNI
    - I will introduce features as soon I will require
- Small
    - This is to have basic features, not incredible posibilies. There are great C++ libraries for that.
- Portability (win/linux)
- zero dependency
    - C++ doesn't have a standard dependency tool. It is going to be a small library, complex dependencies not requiered
- Easy to compile

Performance is desired but not over safety, easy api...



## Safety

Crash reasons and how to deal with it...


Crash  | Deal with...
------------- | -------------
Non trapped exception  | reactive programming with main message loop trapping all exceptions
Null pointer  | generate an exception is better to undefined behaviour (usually crash)
Dangling pointers | smart pointers and signal slot. Please RAII everywhere <br> No arithmetic pointers
Invalid iterators | Exception is better than undefined behaviour (even for deterministic false positives, fail fast)
Memory leaks | same as previous one
Resource manament |  memory is not the only resource. RAII everywhere
Infinite loops | reduce loops ussage
Consuming all resources | _
Integer zero division | throw an exception
Concurrency and paralelims | reduce or remove parallelism <br> do not share, message passing
Float comparison |  don't do it and request for help to the compiler

### Let the compiler help you
I have next flags activated on gcc/g++
> -std=c++14 -O0 -g -Werror -Wall -W -Wundef -Wpointer-arith  -Wfloat-equal -fexceptions -Winit-self -Wconversion  -Wclobbered  -Wempty-body  -Wignored-qualifiers -Wmissing-field-initializers -Wsign-compare -Wtype-limits -Wuninitialized -Wno-unused-result   -Wnon-virtual-dtor -Wreorder -Woverloaded-virtual -Wsign-promo -Winit-self -Wignored-qualifiers -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wshadow -Wcast-qual -Wwrite-strings -Wconversion -time


jle will also provide a base exception class with stack. You will have to fill the stack manually (this is C++)




## Folders

Folder  | Description
------------- | -------------
src  |  source code
src/support  |  _
src/xxx  |  pending
bin  | generated bins
doc  | generated doc
test | source for unit testing
examples  | ex source
data  |  general data


## Compile

Next command are provided

> make

> make help

> make libs

> make doc

> make test

> make compile_test

> make compile_examples



## Roadmap

I don't plan to use it daily. I will write it simultaneously with... web applications with dart, polymer, enjoing Elixir, learning from Go and Rust, following Scala...

In any case, I plan to follow next order (more or less)...

- (done) ~~smart_pointer~~
    - (done) ~~just a safe wrapper over stl~~
- (done) ~~signal_slot~~
- safe containers
- date_time
- exception type with stack
- integer div 0 protection
- double safe comparison
- string tools
- timers
- LL(n) parser
- .ini and .cfg parsing files
- IDL class generation
    - stream
    - yaml
    - json
    - bson
    - less operator
    - equal operator
    - ...
- IDL fsm generation
- async signals
- ...



## Todo

- At this moment... almost all

