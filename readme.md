# Nintendo DS Snake Clone

## Introduction
So this is my first attempt at making NDS Homebrew, and it turned out pretty well I think.
It's obviously written in C++, and since there's no SDL on this console, i'm using libnds, from the devkitPRO developers and NightFox Lib.

## Build
To build this app you'll need a few things :
- GNU make
- the devkitPRO toolchain and the nds-dev package.

Then you simply clone this repository, move into it and type `make`.
The output will be a `.nds` file that you can use on a flashcart, or on an emulator, or even on TWiLight Menu, you choose.

## How to play this game
I really think that I shouldn't make fun of you by explaining you the rules.
There's a twist though, do not eat the purple apples, they're poisonous.