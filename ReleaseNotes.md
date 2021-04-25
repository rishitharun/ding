#	Release Notes - ding_v2.1

**Developer & Maintainer** - [Rishi Tharun](https://linkedin.com/in/rishitharun03) - <<vrishitharunj@gmail.com>><br>
<br>

> Release Date: `25.04.2021`
> 
> NOTE:
> * Only the current version release is pushed to GitHub.
> * Contact the developer for previous version releases.

<br>

Key Points about ding_v2.1
--------------------------
* ding is upgraded to Branch 2 - **ding_v2**
* This is release 1 of Branch 2 - **ding_v2.1**
* ding2 is completely incompatible with ding1
* ding1 is deprecated, and will be left in its incomplete form

<br>

Changes made/Features added in this Release (internal):
-------------------------------------------------------
* ding Shell has been implemented
* Support for operators implemented
* Support for membership functions implemented
* Sourcecode Parsing Techique has been completely revised
* Replaced most of the string comparison based parsing techniques of ding1 with tree-based parsing techniques in ding2


Changes made/Features added in this Release (code prespective):
---------------------------------------------------------------
* `$INTERFACE` block has been deprecated. Instead, an interface function is introduced - `interface({interface_str})`
* Qualifiers have been deprecated.
* Method to assign a member has been changed <br>  ~~`{qualifier} {value/variable} -> {packet}:{member}`~~ &rightarrow; `{packet}.{member} = {value/variable}`
* Method to access a member has been changed <br> ~~`{qualifier} {packet}:{member}`~~ &rightarrow; `{packet}.{member}`
* NumStream values will henceforth be separated by _colon_ "**:**", not by _underscore_ "**_**"
* `mac` type values will henceforth be separated by _tilde_ "**~**", not by _hyphen_ "**-**", which will be used for _minus_
* `bit` type values will henceforth have a "**b**" at the end, to specify that the size is of _bits_ - ~~`32:6`~~ &rightarrow; `32:6b`
* `number` type values will henceforth have a "**B**" at the end, to specify that the size is of _bytes_ - ~~`32:3`~~ &rightarrow; `32:3B`
* Membership functions for *string* datatype have been implemented
* Arithmetic (addition, subtraction) and Relational (lesser than, greater than) operators have been implemented (only for `number` datatype)
* `loop` control structure has been implemented
* Command line arguments implemented
* `receive()` function has been implemented, to receive packets
* Concept of Condition Strings implemented to receive packets based on certain conditions
* `checksum()` function introduced to compute checksums

> This release has all features upto *ding_u1.3*, in addition to the above mentioned features

