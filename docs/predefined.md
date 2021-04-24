
# ding
### - A Programming Language for Customizable Network Traffic Handling
**Developer & Maintainer** - [Rishi Tharun](https://www.github.com/rishitharun) - <<vrishitharunj@gmail.com>><br>
<br>

> For Sample ding codes, check `/scripts/` folder


### List of Datatypes in ding (so far)
*For each datatype, a sample value is given*
  * ip4 &ensp; &ensp; &ensp; &ensp;- `192.168.4.104`
  * mac &ensp; &ensp; &ensp; - `92-48-fe-b7-d5-8c`
  * bit &ensp; &ensp; &ensp; &ensp; - `30:5b` - number after **:** is size in *bits*
  * number &ensp; - `30:1B` - number after **:** is size in *bytes*
  * string &ensp;&ensp;&ensp; - `"Hello World !"`
  * stream&ensp;&ensp;&ensp;- `x0123456789abcdef` - hex variant
  * stream&ensp;&ensp;&ensp;- `12:254:109:33` - decimal variant
  * packet

>*NOTE:<br>Some more datatype may be introduced*<br>
>       *Incomplete Types are not mentioned here*


### List of Pre-defined Functions in ding (so far)
*For each function, the no. of arguments, argument types, and return types are given*
  * __send()__ - any no. of args of type *packet*, and return type is *number*
  * __print()__ - any no. of args of any datatype and return type is *number*
  * __container()__ - 1 arg of type *string*, and return type is *packet*
  * __cat()__ - any no. of args of any type (except *packet*, *bit*), and return type is *string* if all args are *string* type and return type is *stream* otherwise
  * __interface()__ - 1 arg of type *string*, and return type is *number*
  * __receive()__ - 0 (or) 1 arg of type *stream* (condition string), and return type is *packet*
  * __type()__ - 1 arg of type any datatype, and return type is *string*

<br>

#### List of Pre-defined member Functions in ding (so far)
**string**
  * __ip4()__ - 0 args and return type is *ip4*
  * __mac()__ - 0 args and return type is *mac*
  * __bit()__ - 0 args and return type is *bit*
  * __number()__ - 0 args and return type is *number*

>*NOTE:<br>Some more functions may be introduced*<br>
>       *Incomplete Functions are not mentioned here*


#### List of Packets and Members as per ding Naming Conventions:
*Run pacman (PACket MANual) tool*
  `python ./tools/pacman.py`

