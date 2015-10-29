# lib_core

\section smart_pointers


## shared_ptr
Safe shared_ptr wrapper

Same interface as std::shared_ptr but, with no undefined behaviour

links:
 - see: #jle::shared_ptr
 - example: @link /core/ex_shared_ptr.cpp @endlink


## weak_ptr
Safe weak_ptr wrapper

Same interface as std::shared_ptr but, with no undefined behaviour


links:
    - see: #jle::weak_ptr
    - example: @link /core/ex_shared_ptr.cpp @endlink
    - header: @link /core/shared_ptr.hpp  @endlink


\section test

Some macros and functions to help with test core/test.h

See on \ref examples/core/test_shared_ptr.cpp

briefly...

Macro  | Description
------------- | -------------
JLE_TEST_FILE:  |  at the begining of execution file
JLE_TEST_REPORT:  |  at the end of execution file
JLE_TEST_INIT:  |  at the beggining of a test (generally a function)
JLE_TEST_ASSERT:  |  check truth and write a dot
JLE_TEST_ASSERT_NO_DOT:  |  check truth without writting a dot
JLE_TEST_EXCEPTION:  |  check expression throws an exception

links:
    - example: @link /core/ex_test.cpp @endlink
    - header: @link /core/test.h  @endlink


\section string

## Concatenation  JLE_SS

Code like...

~~~~~~~~~~~~~~~~~~{cpp}
std::string s = "asdfds" + var1 + var2 + "\n";
~~~~~~~~~~~~~~~~~~

is very dangerous and limited.

We like...

~~~~~~~~~~~~~~~~~~{cpp}
std::cout << "asdfds" << var1 << var2 << std::endl;
~~~~~~~~~~~~~~~~~~

That is safer and powerful.

Please, never use `+` to concat strings. My proposal is...

~~~~~~~~~~~~~~~~~~{cpp}
JLE_SS("asdfds" << var1 << var2 << std::endl);
~~~~~~~~~~~~~~~~~~

**JLE_SS** will produce a `ostringstream` with all it powers (and safer than +), therefore more expressive



##  Converting

s_try_sxxx  Convert an string to integer, double, duration, time point...

You can provide a value in case conversion fail.
It will return a tuple with the converted value or the default and a boolean indicating if conversion was right

Another utilities are...

- Trimming
- Align
- Splitting


links:
    - example: @link /core/ex_jle_ss.cpp @endlink
    - example: @link /core/ex_jle_string.cpp @endlink
    - header: @link /core/string.h  @endlink





\section signal_slot

An good event system is great in order to compose.
signal_slot is a static event system (good performance and strict compiling type check)

I wrote one years before. I had to deal with optional parameters on templates. This implementation
worked with 0 till 4 parameters. In order to add parameters, was necesary to modify the code.
Very similar code. Yes, it was methaprograming and it was copy/paste (disgusting)

Today, with modern C++, I wrote it with variadic templates. Fantastic experience. Quite easy, and it
works with several parameters

If composition with events is not enought for you, signals are also fully RAII.
When signal or receiver is out of scope, signals are disconnected.
You don't have to take care of disconnection before exit (destructor), neither disconnection to avoid
orphan live references (problem for counter ptrs and garbage collectors)


## Connection syntax recomendations...

Connection  | Syntax
------------- | -------------
signal -> function  |  signal.connect(on_function_name);
signal -> method  |    JLE_CONNECT_METHOD(signal, instance, on_method);
signal -> method on this  |  JLE_CONNECT_THIS(signal, on_method);
signal -> signal  |  signal.connect(&signal_receiver);

## Performance

In my computer with no params, you can emit arround 200.000 signals per second...

Executing...  @link /core/signal_slot_performance.cpp @endlink

     > ---------------------------------------------------------------------
     add int loop

     1783293664
     time: 0.000598493
     calls/millisecond: 1.67086e+06
     ---------------------------------------------------------------------
     signal -> function (emit)

     time: 0.00495689
     calls/millisecond: 201740
     ---------------------------------------------------------------------
     signal -> function (no emit)

     time: 0.00491906
     calls/millisecond: 203291
     ---------------------------------------------------------------------
     signal -> método  (emit)

     time: 0.00461001
     calls/millisecond: 216919
     ---------------------------------------------------------------------
     direct call function

     1000000
     time: 9.1e-08
     too fast...
     ---------------------------------------------------------------------
     signal -> function

     1000000
     time: 0.00556662
     calls/millisecond: 179642


links:
    - see: #jle::signal  #jle::signal_receptor
    - example: @link /core/ex_signal_slot.cpp @endlink
    - header: @link /core/signal_slot.hpp  @endlink

\warning
    **What if you destroy the signal while it is emiting?** <br>
    This is not a specific signal_slot pattern problem <br>
    It's possible to manage this situation in a safe way with signals, but is an incomplete solution. <br>
    The params could be references or raw pointers (please, do not use raw pointers) and they could
    be out of scope when signal is destroyed.
    Providing a partial solution is not a good idea. In case you do something similar,
    a message will be emmited on cerr <br>
    It will be added asynchronous signals. They we'll deal with this situations with no problem, but they will requiere copy on params.

\warning
    **What if an exception is thrown processing a signal?** <br>
    At the moment, it is not managed. The exception will break the control flow not emiting the last connected signals. <br>
    This looks logical. An exception is an exception. I'm thinking about the option on trapping the exception and rethrow a
    new one after finishing signaling. But I'm not sure it is a good idea.

\warning
    **Probably you know a lambda is a function** <br>
    Even when you assign to a variable, the function keep existing and signal is never disconnected automatically (it's a function).

You can connect signals to lambdas (as connecting to functions). <br>
If you are surprised with problems connecting to lambdas who capture scope... this is good. <br>
Capturing scope is not a good idea and new mechanisms are provided in C++14.



\section chrono

**std::chrono** is great, but it's very flexible and therefore verbose.

For realtime applications, I need a precisse monotonic clock.

PC clock is not precisse at all. And system clock syncroniced with ntp is not guaranteed to be monotonic.

**jle::time_point** will be monotonic ant it will syncrhonize with system clock slowly. Except once per day.

using jle::chono (`#include "core/chrono.h"`), is a bit intrusive...

- It will add `using namespace std::literals;` in order to use time literals as `1s + 123ms`
- It will define `operator<<` on `stream` for `duration`

To reduce verbosity, there are proposed some helpers

- `jle::chrono::duration`
- `jle::chrono::now()`
- `jle::chrono::make_from_date(const year&, const month&, const day&)`

As usual, take a look to examples.

\todo micro-adjustments and one big adjustment per day


links:
    - see: #jle::chrono::time_point
    - example: @link /core/ex_chrono.cpp @endlink
    - header: @link /core/chrono.h  @endlink



\section alarm

C++, in case of an exception, will not inform you about the call stack.

We have the exception type. It's great, but we don't have nested calls.

We can not force the compiler to inform about the stack call when an exception is raised, but
we can give you the opportunity to stack errors information.


links:
    - see: #jle::alarm
    - example: @link /core/ex_alarm.cpp @endlink
    - header: @link /core/alarm.h  @endlink



\section   dbl

IEEE754 is great but isn't infinite accurate.

There are two risk.

- Rounds with conmutativity and asociativity operations
- Base ten exact values that are periodic on binary

http://joseluisestebanaparicio.blogspot.com.es/2008/04/el-famoso-formato-ieee754.html

See also:
- jle::log
- jle::pow
- jle::get_decimals10


Using ''-Wfloat-equal'' helps a lot, but we want to compare in a quite safe way. This is jle::dbl for.

links:
    - see: #jle::dbl
    - example: @link /core/ex_dbl.cpp @endlink
    - header: @link /core/dbl.h  @endlink


\section   containers

stl containers are great, but not safe.

Iterators on stl::containers could dangle and they could generate indefined behaviour.

In order to avoid it with moderate performance penalty, jle library will check the validity of iterators with "fail fast" pattern.

It looks a bad idea to keep iterators when the container has been modified, adding or removing elements.
It isn't very expensive to track iterators on these conditions.

jle will throw an exception on invalid iterator operations (instead of undefined behaviour)...

- Iterator pointing a modifyed container (added or removed elements)
- Iterators on empty containers
- Derrefering end iterator
- Comparing iterators from different containers

jle::containers are wrappers to stl containers with iterators verification


See also:



links:
    - jle::vector
    - jle::list
    - jle::map
    - jle::set
    - example: @link /core/ex_container.cpp @endlink
    - headers:
        - @link /core/cont/vector.hpp  @endlink
        - @link /core/cont/list.hpp  @endlink
        - @link /core/cont/map.hpp  @endlink
        - @link /core/cont/set.hpp  @endlink



\section optional

I could contain a value, it could not contain a value...


This is a safe std::experimental::optional wrapper. No undefined behaviour

Some years ago, I wrote something similar with the name of nullable.

The right way to do it, is using algebraic data types. But, we don't have it in C++.

I was thinking of working with algebraic data types with templates and macros, but finally, I decided to postpone it to the sdl to define data structs.

But this is a common, basic, core and very frecuently needed, therefore, here it is the optional type.


links:
 - see: #jle::optional
 - example: @link /core/ex_optional.cpp @endlink


\section tuple

Now we have tuples on C++, but I want to write them at stream.

jle::tuple is not a wrapper, it is just an alias to std::tuple

When you include core/tuple.hpp it will inject ostream << support on tuples.

As this is a bit invasive, we created the alias jle::tuple. If you see jle::tuple, you know it has ostream<< suport


links:
 - see: core/tuple.hpp
 - example: @link /core/ex_tuple.cpp @endlink



 \section timers

 It is part or message driven programming.

 Only one line is necessary to configure a timer, check the example.

 links:
  - see: core/timer.h
  - example: @link /core/ex_timer.cpp @endlink


 \section synchr

I don't try to run several threads in a safe way in C++

In fact, I do the opposite.

With this library, you can run multiple threads sequentially.

If you use a third party library with other thread callbacks, or if you want to run passive waiting (and you need a thread), you can call JLE_SYNCR to avoid having more than one thread running simultaneously.

Also you have here the way to start and stop the jle mail loop.

 links:
  - see: core/synchr.h
  - example: @link /core/ex_synchr.cpp @endlink


  \section int_div0

  In most hardware platforms, dividing by 0 two integers is a big problem.

  On x86, it stops the program without exception or any warning.

  To avoid this "undefined behaviour" with bad result, you can use

  JLE_HANDLE_INTDIV0 macros. See the example.



  links:
   - see: core/int_div0.h
   - example: @link /core/ex_int_div0.cpp @endlink
