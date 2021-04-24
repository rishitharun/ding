
# ding
### - A Programming Language for Customizable Network Traffic Handling

**Version** - ding_v2.1

**Developer & Maintainer** - [Rishi Tharun](https://www.github.com/rishitharun) - <<vrishitharunj@gmail.com>><br>
<br>
>NOTE:
> * Target system is **LINUX** only - Not portable
> * Root access is required to run
> * Requires **Python 3.x** and **GNU Compiler Collection**
> * Install python3 and gcc before proceeding to build

<br>

### About Version Numbering and Releases
* The version numbering method followed is **ding_v(branch).(release).(minor_release [optional] )**
* Branch change occurs when there is an appreciable amount of architectural design and implementation changes
* Branch change will occur rarely, only if the requirement is absolute
* Each release occurs after a some amount of minor releases (or) patches (or) fixes
* Each release will be fixed by the developer
* Minor Releases will be the fixes or features that are immediately required in the mainline code, and that could not wait till the major release
* Current release version is **ding_v2.1**
* Any references of ding version as **ding_uX.X** (with a **u** instead of **v**) corresponds to unstable development snapshots
* A Comprehensive Developer's Guide is not yet prepared. Please contact the developer to know more.

<br>

## About ding:
  *ding* is a network-specific programming language, used for *customizable network traffic handling.*
  This can be used to create, send, receive customisable network packets, and perform customized
  real time traffic handling with other network devices. ding is written entirely in C. Support tools
  are written in python. Currently, as of this release, the project is still under development.

> *Handle the traffic on you own, as you wish !*

<br>

### Translator Type:
  INTERPRETER

<br>

### To Build:
  `python ./tools/build.py`
<br> Copy to /bin/: `cp ./ding /bin/`

<br>

### To Run:
  `ding <sourcefile>`

<br>

### To know about ding Naming Conventions for Packets and Members:
  `python ./tools/pacman.py`

<br>

### To invoke Shell:
  `ding`

<br>

### Program Strcuture:
1. Three blocks
    * **".decl:"** --> To declare variables; No definitions allowed
    * **".func:"** --> To define functions; _**NOT HANDLED**_
    * **".flow:"** --> Program execution entrypoint; _**PARTIALLY HANDLED**_
2. Comments
    - Characters succeeding the **!** character in a line are treated as Comments;
    - __NO__ Multi-line comments
    - Documentation text can be given above the very firat block in the program
<br>

### Sample ding Codes:
  Check `/scripts/` folder

<br>

### How to code:
  Check `howto.txt` and `/scripts/*.ding`


