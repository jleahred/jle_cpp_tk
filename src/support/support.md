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

- **JLE_TEST_FILE:** at the begining of execution file
- **JLE_TEST_REPORT:** at the end of execution file
- **JLE_TEST_INIT:** at the beggining of a test (generally a function)
- **JLE_TEST_ASSERT:** check truth and write a dot
- **JLE_TEST_ASSERT_NO_DOT:** check truth without writting a dot
- **JLE_TEST_EXCEPTION:** check expression throws an exception

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
