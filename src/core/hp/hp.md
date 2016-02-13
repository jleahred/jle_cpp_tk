# hp & AST


Full example on examples/project/idl


## Aim

Enjoy!

A parser is an important tool for a programmer.

I'm also interested on *methaprograming*. One one to do it, and sometimes
the only correct one, is creating *DSLs*.

In most cases, these *DSLs* has to be external. Working with external *DSLs*
give you full freedom.



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



## Defining a grammar


### Basic grammars

Let's start with a common and simple exercise, parsing a math equation.

Lets start with hello world on grammars...

![Image](hello_world1.png)

This grammar, accepts one or several 'a' char. Simple.

The first line, defines the initial *non terminal* symbol

**NON Terminal symbols** will be on upper case

**Terminal symbols** will be on lower case

As you can see, it derives by left.

Now, let's have a sequence of 'a' followed by 'b' and a sequence of 'a' again
Lets try next one, parenthesis...

It has to accept things like...

~~~~~~~
abaaa
aaaabaa
aba
~~~~~~~

~~~~~~~
B

B ::= A  b  A

A ::= a A
A ::= a

a ::= 'a'
b ::= 'b'
~~~~~~~
Great.


But now, we want same quantity of 'a' at the beginning and at the end.

It has to accept things like...

~~~~~~~
aaabaaa
aaaabaaa
aba
~~~~~~~

But not...

~~~~~~~
aabaaa
aaaaba
ab
~~~~~~~
Let's try

~~~~~~~
B

B ::= a  b  a
B ::= aa  b  aa
B ::= aaa  b  aaa


a ::= 'a'
b ::= 'b'
aa ::= 'aa'
aaa ::= 'aaa'
~~~~~~~

From now on, lasts lines will indicate a valid input, rest of lines will be the grammar

This is not a full solution, and it's not an elegant one either.

~~~~~~~
B

B ::= a  B  a
B ::= b

a ::= 'a'
b ::= 'b'

input: aaabaaa
input: aba
~~~~~~~

This looks great. Generic, concise, simple.

We can move it to parenthesis...

~~~~~~~
B

B ::= ( B )
B ::= b

b ::= 'b'
( ::= '('
) ::= ')'

input: (((b)))
input: (b)
~~~~~~~

### Expressions grammar

Let's start with numbers.

~~~~~~~
EXPR

EXPR ::=  NUM

NUM  ::=  d NUM
NUM  ::=  d

d ::= ([0-9])
input: 123456
~~~~~~~

Terminal symbols can be defined as *regular expressions* in order to simplify the rule.

To keep the example simple, we will let numbers of any size and just integers.

If terminal symbols can be written as *regular expressions*, then we can simplify...

~~~~~~~
EXPR

EXPR ::=  num

num ::= ([0-9]+)

input: 123456
~~~~~~~

Added one operator
~~~~~~~
EXPR

EXPR ::=  num operator num

num ::= ([0-9]+)
operator ::= ([\+|\-|\*|\/])

input: 1+2
~~~~~~~

But expressions has to accept multiple operators and numbers...

~~~~~~~
EXPR

EXPR ::=  num operator EXPR
EXPR ::=  num

num ::= ([0-9]+)
operator ::= ([\+|\-|\*|\/])

input: 1+2*3
~~~~~~~


And what about the parenthesis?...
~~~~~~~
EXPR

EXPR ::=  ( EXPR ) operator EXPR
EXPR ::=  ( EXPR )
EXPR ::=  num operator EXPR
EXPR ::=  num

num      ::= ([0-9]+)
operator ::= ([\+|\-|\*|\/])
(        ::= '('
)        ::= ')'

input: (1+2)*3
input: (1*(3+2))*3+(8*9)
~~~~~~~

We could want to let spaces between elements.

~~~~~~~
EXPR

EXPR ::=  _ ( _ EXPR _ ) _ operator _ EXPR
EXPR ::=  ( _ EXPR _ )
EXPR ::=  num _ operator _ EXPR
EXPR ::=  num

num      ::= ([0-9]+)
operator ::= ([\+|\-|\*|\/])
(        ::= '('
)        ::= ')'
_        ::= ([ |\t]*)

input: ( 1+2 ) *3
input: (1* (3  +2 ) )* 3+( 8* 9  )
~~~~~~~

This grammar will produce next tree for entrance ```(1* (3  +2 ) )* 3+( 8* 9  )```

![Image](expression_simple.png)

Fantastic, but, what if we want to consider operator priority?...

Here it is...

~~~~~~~
EXPR

EXPR    ::=    _ unaryoperator _ ADDS _
EXPR    ::=    ADDS
ADDS    ::=    FACTS _ add_operator _ ADDS
ADDS    ::=    FACTS
FACTS   ::=    VAL _ mult_operator _ ADDS
FACTS   ::=    VAL

VAL     ::=    FUNC _ ( _ EXPR _ )
VAL     ::=    _ num
VAL     ::=    VAR
VAR     ::=    _ id
FUNC    ::=    _ id

FACTS   ::=    _ ( _ EXPR _ )


num           ::=    ([0-9]+)
id            ::=    ([a-z|A-Z][0-9|a-z|A-Z|_]*)
id            ::=    (_+[0-9|a-z|A-Z]+[0-9|a-z|A-Z|_]*)
mult_operator ::=    ([\*|\\])
add_operator  ::=    ([\+|\-])
unaryoperator ::=    ([\+|\-])
_             ::=    ([ |\t]*)
(             ::=    (\()
)             ::=    (\))
~~~~~~~

And here is the tree with correct priority for input ```1+2*3```...

![Image](expresion_priority.png)



## Transforming text

Validating files is quite interesting.

Parsing files and generating *AST* is even better.

Once you have the *AST*, you can do things depending of the input file.

Great!!!!

But many times, the result will be text, perhaps, another file.

Even when is not the case, generating a normalized text is an interesting option (some times a very good one) to process the input.

Generating *DSLs* is a good example.

That's the main reason why I build this lib, and in these cases, the game consists
on getting a text, validate it, and generating a different text.

The output could be also... a program in pcode or c++, or embedded language.

Yes! this is external DSL

Lets see one example with the expression grammar.

Look to ```##transf2->```

```
MAIN

MAIN ::= EXPR

EXPR    ::=    _ VAR _ = _ EXPR                                 ##transf2-> $(EXPR)$(__endl__)copy2:$(VAR)
EXPR    ::=    _ VAL _ EXPR'                                    ##transf2-> $(VAL)$(__endl__)$(EXPR')
EXPR    ::=    _ unaryoperator VAL _ EXPR'                      ##transf2-> $(VAL)$(__endl__)$(unaryoperator)$(__endl__)$(EXPR')
EXPR    ::=    _ ( _ EXPR _ ) _ EXPR'                           ##transf2-> $(EXPR)$(__endl__)$(EXPR')
EXPR    ::=    _ unaryoperator ( _ EXPR _ ) _ EXPR'             ##transf2-> $(EXPR)$(__endl__)$(unaryoperator)$(__endl__)$(EXPR')
EXPR'   ::=    POWER
EXPR'   ::=    FACTOR
EXPR'   ::=    SUM

POWER   ::=    _ powerop _ VAL _ POWER                          ##transf2-> $(VAL)$(__endl__)$(POWER)$(__endl__)$(powerop)
POWER   ::=    _ powerop _ VAL _ FACTOR                         ##transf2-> $(VAL)$(__endl__)$(powerop)$(__endl__)$(FACTOR)
POWER   ::=    _ powerop _ VAL _ SUM                            ##transf2-> $(VAL)$(__endl__)$(powerop)$(__endl__)$(SUM)
POWER   ::=    _ powerop _ ( _ EXPR _ ) _ EXPR'                 ##transf2-> $(EXPR)$(__endl__)$(mult_operator)$(__endl__)$(EXPR')

FACTOR  ::=    _ mult_operator _ VAL _ POWER                    ##transf2-> $(VAL)$(__endl__)$(POWER)$(__endl__)$(mult_operator)
FACTOR  ::=    _ mult_operator _ VAL _ FACTOR                   ##transf2-> $(VAL)$(__endl__)$(mult_operator)$(__endl__)$(FACTOR)
FACTOR  ::=    _ mult_operator _ VAL _ SUM                      ##transf2-> $(VAL)$(__endl__)$(mult_operator)$(__endl__)$(SUM)
FACTOR  ::=    _ mult_operator _ ( _ EXPR _ ) _ EXPR'           ##transf2-> $(EXPR)$(__endl__)$(mult_operator)$(__endl__)$(EXPR')

SUM     ::=    _ add_operator _ VAL _ POWER                     ##transf2-> $(VAL)$(__endl__)$(POWER)$(__endl__)$(add_operator)
SUM     ::=    _ add_operator _ VAL _ FACTOR                    ##transf2-> $(VAL)$(__endl__)$(FACTOR)$(__endl__)$(add_operator)
SUM     ::=    _ add_operator _ VAL _ SUM                       ##transf2-> $(VAL)$(__endl__)$(add_operator)$(__endl__)$(SUM)
SUM     ::=    _ add_operator _ ( _ EXPR _ ) _ EXPR'            ##transf2-> $(EXPR)$(__endl__)$(add_operator)$(__endl__)$(EXPR')
SUM     ::=    _                 

VAL     ::=    FUNC _ ( _ EXPR _ ) _                            ##transf2-> $(EXPR)fun/1:$(FUNC)
VAL     ::=    FUNC _ ( _ EXPR _ , _ EXPR _ ) _                 ##transf2-> $(EXPR)$(__endl__)$(EXPR#1)fun/2:$(FUNC)

VAL     ::=    num                            
VAL     ::=    VAR                              
VAR     ::=    id                                               ##transf2-> var:$(id)
FUNC    ::=    id                             

num           ::=    ([0-9]*\.[0-9]+)                           ##transf2-> num:$(t)
num           ::=    ([0-9]+\.[0-9]*)                           ##transf2-> num:$(t)
num           ::=    ([0-9]+)                                   ##transf2-> num:$(t)
id            ::=    ([a-z|A-Z][0-9|a-z|A-Z|_]*)  
id            ::=    (_+[0-9|a-z|A-Z]+[0-9|a-z|A-Z|_]*)
powerop       ::=    (\^)                                      ##transf2-> fun/2:$(t)
mult_operator ::=    ([\*|\/])                                 ##transf2-> fun/2:$(t)
add_operator  ::=    ([\+|\-])                                 ##transf2-> fun/2:$(t)
unaryoperator ::=    ([\+|\-])                                 ##transf2-> fun/1:$(t)
_             ::=    ([ |\t]*)                                 ##transf2-> $(__nothing__)
(             ::=    (\()                        
)             ::=    (\))                        
,             ::=    (,)                          
=             ::=    (=)                          
```


And it will generate for input ```1+2*3 +(7/9*5) +1```...

```
num:1
num:2
num:3
fun/2:*
num:7
num:9
fun/2:/
num:5
fun/2:*

fun/2:+
num:1
fun/2:+

fun/2:+
```

A small program, easy to process
