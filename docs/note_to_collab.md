
# ding
### - A Programming Language for Customizable Network Traffic Handling
**Developer & Maintainer** - [Rishi Tharun](https://www.github.com/rishitharun) - <<vrishitharunj@gmail.com>><br>
<br>

### Note To Contributors:
#### Note To Developers:
* A Comprehensive Developer's guide has been not been created yet.
* Please contact the developer for to understand more about the architecture and implementation.
* Developer's guide will be created soon.

#### Note To Testers:
* Please report any issues that you find.
* You can report any unexpected behavior exhibited by the interpreter.
* Please go through the howto.md and sample scripts to understand how to code in ding.
* Try out different scenarios.
* Some sample scenarios which can be reported as valid issues are,
   * Any memory leak/corruption issues, segmentation faults
   * Unexpected output for any correct ding code
   * Program not terminating gracefully, for any wrong ding code
   * Program not conveying proper exit message, for any wrong ding code
   * Any **DEBUG INFO** messages are printed on the console, then that is considered to be a potential issue
* Testers can enable logging, to collect logs.
* Logging feature is not done completely. As of now, some of the important functionalities has logging enabled.
* Further logs will be introduced during development.
* If some logging is necessary and not yet enabled, testers can contact the developers to enable them immediately.

* Logging, by defaut, is disabled to avoid extra time being spent to write logs.
* To enable logging, please build the binary, after setting the logging type in `../include/logger.h`.
* As of now, there are some logging types defined, so that only the necessary logs can be collected.
* If all the logs are needed, then set the logging type as `LOG_ALL`.
* Please do report any of the logging issues that you can find.

* Memory handling testing is one of the important testing that is to be done, to verify whether internally memory is handled properly or not.
* Memory handling testing can be done from verifying memory handling logs.
* Please contact the devloper to know more regarding memory handling testing.
* If possible, please support the developer in developing an automated test suite for the memory handling testcases

##### Test Plans
* Feature Sanity - Testplan TBD
* Functional Coverage - Testplan TBD
* Memory handling Coverage - Testplan TBD
