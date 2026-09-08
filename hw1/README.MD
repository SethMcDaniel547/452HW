# HW1

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

**Run within *root* folder**
```
make test
```

## Running valgrind
Runs valgrind on the example usage of the queue

**Run within *src***
```
make valgrind
```

## Project overview
The main code to look at is my implementation of src/deq.c

The tests are found at tests/test_deq.c

I have made minor modifications to the make files to get the project to compile and to add a convient way to test. Innitally I had made a testing framework by hand, I refactored it to use Unity as I felt that was more effective.

I did not write any of the code within tests/unity and do not claim any ownership. That is from their site that i've listed below. I chose this as its highly portable, meaning that I can have it work on onyx as well as my linux dev environment.

## References/Included Code
This where I got the Unity Testing For C Framework

https://www.throwtheswitch.org/unity#unity-get-section