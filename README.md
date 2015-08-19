# README


[(online doc)](http://jleahred.github.io/jle_cpp_tk.doc/index.html)


<a href="https://scan.coverity.com/projects/5356">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/5356/badge.svg"/>
</a>

## Modifs

...soon





## About
### Why

C++ is not the safest programing language in the world. In fact, it's easy to produce *undefined behaviour* working with C++.
In several cases, it would be interesting to work in a safe way (no body like crashes).
Working in a team (generally diferent levels), it's also desirable to have safety in mind.


Some years ago I wrote a C++ toolkit with regular expressions, tuples, class definition IDL, fsm IDL, config files. LL(n) parser, signal-slot, monotonic date time with adjustments...

These last years, C++'s world has moved a lot.

Now gcc and other compilers run a lot of features from C++11, C++14 and partially from C++17

Some of the tools I wrote or added to my library, are not necessary any more.
The implementation and API can also be improved.

For example... I wrote a smart_pointer called CountPtr. I also wrote code to manage tuples,
I wrote a beautifull, platform independent, safe, and easy to use classes to deal with date_time and durations,
I added a small wrapper library for PCRE regular expressions. These features have been added to C++

Now, we also have type inference, lambdas, variadic templates, initializer lists...

It's time to enjoy and write a new set of tools based on modern C++ (more reduced than previous one, but with a better language)

Previously I wrote the documentation with **asciidoc**, muse over emacs and doxys. Now I'm going to do it with doxygen (I'm not a fan of these kind of tools, but it's a standard).


### Directives

My aim is different from C++.

- Safety
    - I don't like undefined behaviour
    - I don't like crashes due to programming mistakes
    - C++ is a dangerous language. It's easy to have crashes, I want a library protecting and helping the programmer.
    - Reactive programming
    - If we have a fail processing a message, usually is better to inform about it and continue with next one.
- KISS and YAGNI
    - Easy to use API (the implementation could be complex)
    - I will introduce features as soon I will require
    - No thousands of options, better few options, easy to use and less verbose
- Make dificult to write only code using this library. The code using this library...
    - could not be simple to write
    - has to be easy to read
- Small
    - This is to have basic features, not incredible posibilies. There are great C++ libraries for that.
- Portability (win/linux)
- zero dependency
    - C++ doesn't have a standard dependency tool. It is going to be a small library, complex dependencies not requiered
- Easy to compile

Performance is desired but not over safety, easy api...



## Safety

C++ is focused on performance and low level abstractions.

In C++ you can have *undefined behaviour* for several errors.

Most of these undefined behaviours ends with a program crash.

My aim is to reduce program crashes. I want my program keep running even after an error.

I don't try to transform C++ in Erlang or Elixir.
And yes, I know that Rust is the tool wich try to be efficient,
close to metal and  with no undefined behaviour.

I enjoy learning and practicing Erlang/elixir. I learnt several concepts with them.
Rust looks quite interesting and promising, we will see what happens in the future, meanwhile,
I'm learning and following it.

Crash and errors reasons and how to deal with them...


Crash  | Deal with...
------------- | -------------
Non trapped exception  | reactive programming with main message loop trapping all exceptions
Null pointer  | generate an exception is better to undefined behaviour (usually crash)
Dangling pointers | smart pointers and signal slot. Please RAII everywhere <br> No arithmetic pointers
Memory leaks | same as previous one
Resource manament |  memory is not the only resource. RAII everywhere
Invalid iterators | Exception is better than undefined behaviour (even for deterministic false positives, fail fast)
Infinite loops | reduce loops ussage
Consuming all resources | _
Integer zero division | throw an exception
Concurrency and paralelims | reduce or remove parallelism <br> do not share, message passing
Float comparison |  don't do it and request for help to the compiler
Race conditions | Share nothing, send messages, high level concurrency tools... or reduce paralelism


### Let the compiler help you
I have next flags activated on gcc/g++
> -std=c++14 -O0 -g -Werror -Wall -W -Wundef -Wpointer-arith  -Wfloat-equal -fexceptions -Winit-self -Wconversion  -Wclobbered  -Wempty-body  -Wignored-qualifiers -Wmissing-field-initializers -Wsign-compare -Wtype-limits -Wuninitialized -Wno-unused-result   -Wnon-virtual-dtor -Wreorder -Woverloaded-virtual -Wsign-promo -Winit-self -Wignored-qualifiers -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wshadow -Wcast-qual -Wwrite-strings -Wconversion -time


jle will also provide a base exception class with stack. You will have to fill the stack manually (this is C++)




## Folders

Folder  | Description
------------- | -------------
src  |  source code
src/core  |  Basic tools (signal-slot, strings...)
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
    - (done) ~~just a safe wrapper over stl, but safer~~
- (done) ~~signal_slot~~
- (90%) date_time
- (done) ~~string tools~~
- (done) ~~exception type with stack~~
- (done) ~~double safe comparison~~
- (done) ~~safe containers~~
- (done) ~~nullable type, it is a wrapper from std::experimental::optional~~
- (done) ~~tuples ostream <<~~
- integer div 0 protection
- LL(n) parser
- qt gui for LL(n)
- .ini and .cfg parsing files
- Reactive message oriented: synchr, timers
- ashared_ptr. Destroy it ansynchronous way to avoid destroy when using it
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
- soft-realtime facilities
- paralelism helpers
- factory template
- ...



## Todo

- At this moment... almost all

