# hp & AST


Full example on examples/project/idl


## Aim

Enjoy!

A parser is an important tool for a programmer.

I'm also interested on *methaprograming*. One one to do it, and sometimes
the only correct one, is creating *DSLs*.

In most cases, these *DSLs* has to be external. Working with external *DSLs*
you have full freedom.



## Tools

On `tools` folder we have *hpt* binary. It works with hp and *AST* to process inputs with grammars and generates files.

On `tools/qt` we have a small *ui* to work with grammars and templates


## Main actors

### hp

This is a simple LL(n) parser.

With it we can validate inputs with a specific grammar.

This is dynamic, therefore not fast.

It will create an *AST*



### AST

As a result of parsing a file with a grammar, an *AST* will
be generated.

We can process this *AST* with a *c++* program, or we can use the *templates*
transformation.


### Templates

A template is a piece of text with functions calls.

These functions will transform the parameters, create variables and they also
will return a *template*



## Transforming text

Validating files is quite intereseting.

Parsing files and generating *AST* is even better.

Once you have the *AST*, you can do things depending of the input file.

Great!!!!

But many times, the result will be text, perhaps, another file.

Even when is not the case, generating a normalized text is an interesting option (some times a very good one) to process the input.

Generating *DSLs* is a good example.

That's the main reason why I build this lib, and in these cases, the game consists
on getting a text, validate it, and generating a different text.


## Defining a grammar

Let's start with a common and simple exercise, parsing a math equation.

Lets start validating numbers in float precision.
