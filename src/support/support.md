# libs

\section support

\subsection smart_pointers


### shared_ptr
Safe shared_ptr wrapper

links:
 - see: #jle::shared_ptr
 - example: @link /support/shared_ptr.cpp @endlink


### weak_ptr
Safe weak_ptr wrapper


links:
    - see: #jle::weak_ptr
    - example: @link /support/shared_ptr.cpp @endlink
    - header: @link /support/shared_ptr.hpp  @endlink


\subsection test

Some macros and functions to help with test support/test.h

See on \ref examples/support/test_shared_ptr.cpp

briefly...

Macro  | Description
------------- | -------------
JLE_TEST_header:  |  at the begining of execution file
JLE_TEST_REPORT:  |  at the end of execution file
JLE_TEST_INIT:  |  at the beggining of a test (generally a function)
JLE_TEST_ASSERT:  |  check truth and write a dot
JLE_TEST_ASSERT_NO_DOT:  |  check truth without writting a dot
JLE_TEST_EXCEPTION:  |  check expression throws an exception

links:
    - example: @link /support/test.cpp @endlink
    - header: @link /support/test.h  @endlink


\subsection string

### Concatenation  JLE_SS

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

links:
    - example: @link /support/jle_ss.cpp @endlink
    - header: @link /support/string.h  @endlink



\subsection signal_slot

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


### Connection syntax recomendations...

Connection  | Syntax
------------- | -------------
signal -> function  |  signal.connect(on_function_name);
signal -> method  |    JLE_CONNECT_INSTANCE(signal, instance, on_method);
signal -> method on this  |  JLE_CONNECT_THIS(signal, on_method);
signal -> signal  |  signal.connect(&signal_receiver);

### Performance

In my computer with no params, you can emit arround 200.000 signals per second...

Executing...  @link /support/signal_slot_performance.cpp @endlink

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
    - example: @link /support/signal_slot.cpp @endlink
    - header: @link /support/signal_slot.hpp  @endlink

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
