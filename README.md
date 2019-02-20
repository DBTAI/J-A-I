# J-A-I

JesseAI Version 1.75 Release Information:

Version 1.75, in comparison to other updates is just that of an additional implant of of memory. Detailing
of 357 conversational lines of dialogue. Equaling now that she has a total amount of conversational dialogue
of 1,029. With that being said, I have also shrunken the title screen and removed all unwanted text, leaving
only the current version number. In the hopes of saving space on the screen for the user.


Quote:
Quote:

You can compile on the command line with: 

g++ -O3 -Wall -m64 -std=c++17 -I[g++ toolchain location] -c "Jesse 1.75.cpp"  

Just be sure to replace "[g++ toolchain location] with the actual location of your g++ toolchain location (where the binary files are at).



This project was written in C++ ver 17, using the Code::Blocks IDE. 

I used the latest MinGW toolchain:  MinGW-W64 GCC-8.1.0

You can download the toolchain at: https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/

I use the version that reads: x86_64-win32-seh

The "x86_64" is the 64-Bit version, but if you are on a 32-Bit machine, then use the "i686" version.

On the x86_64 (64-Bit) versions, you will notice "posix" and "win32". From what I understand, the "posix" is defined for UNIX, so I do not use it. I suggest using the "win32" version for Windows.

You will also notice the tailing "sjlj" and "seh" versions in the list. SJLJ means, "setjmp/longjmp" and SEH means, "Structured Exception Handling". 

SJLJ is an older exception handling and causes you apps to run slower (though you may not notice it in JesseAI at this time) due to it incuring a minor performance penalty. A few sites I have read about it show how an app may silently end without errors during some exceptions. See the links below for more details.

SEH is fully available in 64-Bit GCC version 4.8 and later. This is the one I suggest to use.

Since I don't have the opportunity to use Linux at this time, a buddy of mine will help me in that department with JesseAI 2.0 and later.


Here are a few links for more information about the MinGW toolchain and the verious versions:


https://stackoverflow.com/questions/15670169/what-is-difference-between-sjlj-vs-dwarf-vs-seh

https://stackoverflow.com/questions/30739099/what-is-the-difference-between-mingw-seh-and-mingw-sjlj/30739394

http://www.robelle.com/smugbook/posix.html

https://en.wikipedia.org/wiki/POSIX

https://docs.microsoft.com/en-us/windows/desktop/debug/structured-exception-handling

https://docs.microsoft.com/en-us/cpp/cpp/structured-exception-handling-c-cpp?view=vs-2017

https://stackoverflow.com/questions/2782915/what-should-i-know-about-structured-exceptions-seh-in-c

https://www.itprotoday.com/security/structured-exception-handling-and-security
