# libs

\section support

\subsection smart_pointers


### shared_ptr
Safe shared_ptr wrapper see #jle::shared_ptr

### weak_ptr
Safe weak_ptr wrapper see #jle::weak_ptr


\subsection test

Some macros and functions to help with test support/test.h

See on \ref examples/support/test_shared_ptr.cpp

briefly...

Macro  | Description
------------- | -------------
JLE_TEST_FILE:  |  at the begining of execution file
JLE_TEST_REPORT:  |  at the end of execution file
JLE_TEST_INIT:  |  at the beggining of a test (generally a function)
JLE_TEST_ASSERT:  |  check truth and write a dot
JLE_TEST_ASSERT_NO_DOT:  |  check truth without writting a dot
JLE_TEST_EXCEPTION:  |  check expression throws an exception

Example: @link /support/test.cpp @endlink


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

**JLE_SS** will produce a `ostringstream` with all it powers (and safer), therefore more expressive

Example: @link /support/jle_ss.cpp @endlink


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


Connection syntax recomendations...

Connection  | Syntax
------------- | -------------
signal -> function  |  signal.connect(on_function_name);
signal -> method  |    JLE_CONNECT_INSTANCE(signal, instance, on_method);
signal -> method on this  |  JLE_CONNECT_THIS(signal, on_method);
signal -> signal  |  signal.connect(&signal_receiver);

Example: @link /support/signal_slot.cpp @endlink
