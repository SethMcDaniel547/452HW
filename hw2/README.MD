# HW2

Name: Seth McDaniel

Email: sethmcdaniel547@u.boisestate.edu

Class: CS452-1

## Usage

### Running example file
This runs main after its compiled and simply showcases that the double ended queue works.

**Run within *src***
```
make run
```


### Running tests
The tests are built using the unity testing framework

**Run within *tests* folder**

*Runs general tests*
```
make run
```
*Runs additional more specfic tests, like utils and specfic freelist methods*
```
make modules
make run-modules
```

### Cleaning Files
Run in either src or tests depending on what you are doing
```
make clean
```

## Project overview
The main code to look at is within src, mainly freelist, utils, and balloc. The main.c file when ran uses a slightly altered version of my main.c from HW1 to show off the deq usage. There is a deq folder which is an unmodified version of my hw1 deq.

One thing to note is that if you pass in a non-power of 2 number for bcreate, I do just round it up to the next power of 2.

The tests are found at tests/test_balloc.c

I did not write any of the code within tests/unity and do not claim any ownership. That is from their site that i've listed below. I chose this as its highly portable, meaning that I can have it work on onyx as well as my linux dev environment.

## References/Included Code
This where I got the Unity Testing For C Framework

https://www.throwtheswitch.org/unity#unity-get-section