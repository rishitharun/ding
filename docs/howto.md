
# ding
### - A Programming Language for Customizable Network Traffic Handling
**Developer & Maintainer** - [Rishi Tharun](https://linkedin.com/in/rishitharun03) - <<vrishitharunj@gmail.com>><br>
<br>

> NOTE:
> * Syntax given using the curly brackets **{** and **}** are only for understanding purpose.
> * No curly brackets required when writing ding code.
> * Check [`../scripts/*.ding`](https://github.com/rishitharun/ding/blob/main/scripts) for sample ding codes

<br>

### How to CODE in ding:
  * Comments are given after "**!**"
  * *ONLY* single line comments
  * Documentation text can be given above the first block in the program
  * Indentation and usage of whitespaces, and tabspaces are allowed anywhere
  * The line delimiter will be the new line feed.

  * Program Divided into three blocks: **.decl:** , **.func:** , **.flow:**
	
  * decl block will have all the variable declaration
  * **NO DEFINITION** is allowed in decl block
  * Declaration of variables must be as <br>`{datatype} {identifier}`

  * Multiple variables of same type can be declared with a comma <br> `{datatype} {identifier1}, {identifier2}`

  * func block will have the user defined functions
  * The func block is not handled as of now. Better leave it empty.
  * The code inside func will not be processed
	
  * flow block will be the entry point of program execution
  * Definitions of variables, function calls, membership assignments, operations can be done in flow block

  * Variable definitions can be done by assignments

  * IP addresses must be separated by "**.**" - `127.0.0.1`
  * MAC addresses must be separated by "**~**" - `ca~fe~ba~be~f0~0d`
  * Numbers will contain two parts separated by a "**:**" and ending with "**B**" - `32:2B`
  * First portion is the value, second portion is the number of bytes needed to be allocated. Upto 8 Bytes is allowed.
  * Bits will contain two parts separated by a "**:**" and ending with "**b**" - `9:5b`
  * First portion is the value, second portion is the number of bits needed to be allocated.
  * Strings must be enclosed within double quotes '**"**' <br> `print("Have a Nice Day!")`

  * Stream type values can be either decimal or hexadecimal numerals
  * Stream values in decimal must be separated by "**:**" - `12:255:90:42`
  * Stream values in hexadecimal must start with an "**x**" - `xfeda`

  * All variables will be treated as objects, and they will have their own member variables and functions
  * To access a member variable, the syntax must be as, <br> `{variable}.{member_variable}`
  * To access a member function, the syntax must be as, <br> `{variable}.{memberfunction( {args} )}`

  * Function calls will usually accept any already declared variables or raw values or packet members <br> `print( {args} )`

  * Functions can also have other functions as arguments <br> `print({any_other_function})`
  * Arithmetic (addition, subtraction) and relational (lesser than, greater than) operators can be used only between `number` datatypes

  * Condition Strings are actually stream variables, represented in a string format. They cannot be assigned to strings
  * Condition Strings are similar to normal strings, but prefixed with a "**c**" <br> `c" {condition_string} "`
  * Packet representation in condition string follows bottom-to-top order in the OSI layers, i.e., the datalink layer is given first, followed by network, and so on
  * Each packet name in condition string is specified as defined by ding naming conventions, separated by a space  <br> `c"ETHER() IP4() TCP()"`
  * Each packet in a condition string will have a set of paranthesis,that encloses their member values
  * Members in condition string are separated by _comma_ "**,**"
  * Member names in condition string are specified as defined by ding naming conventions, and values are specified by assigning it to the members
  <br> `c"ETHER(SRC_MAC = ca~fe~ba~be~f0~0d, DEST_MAC = 12~34~56~78~90~fa) IP4(SRC_IP=127.0.0.1)"`
  * Members in condition string can also be assigned to any already declared variables <br> `c"ETHER(SRC_MAC = {any_variable})"`
  * Condition strings can be passed as argument to `receive()` to receive packets as specified in the string <br> `receive( {cond_str} )`
<br>

**NOTE** : To know about ding naming conventions for packets and members, use pacman (PACket MANual) tool <br> `python ./tools/pacman.py`

<br>

### How to USE in ding Shell:
  * Open ding Shell.
  * The shell prompt will be a word inside paranthesis, which indicates the current block you are in.
  `(_ding_)` or `(.decl:)` or `(.func:)` or `(.flow:)`

  * The `(_ding_)` block can be considered as a comment section.
  * Whatever is typed in the block will be ignored, except for the other block keywords or exit keyword.

  * To switch between blocks, type the corresponding block that you need to switch to.
  * For example, to switch to `.flow:`, type `.flow:` in the prompt. <br>

		(.decl:)
		(.decl:) .flow:
		
		(.flow:)
  * To exit the shell, type _exit_ in any block

