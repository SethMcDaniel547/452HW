# HW4

Name: Seth McDaniel

Email: sethmcdaniel547@u.boisestate.edu

Class: CS452-1

## Usage

### Running program

*Run within root folder*
```
make
./wam
```


### Running checks

**Run within root folder**

*Output some debug information and ensures the program works in non gui environments*
```
make check
```

### Cleaning Files
Run in root folder
```
make clean
```

### Running valgrind

Run in root folder
```
make
./vg
```

## Project overview
This updates the existing whack-a-mole game to be (more) multithreaded (technically the gui already was but now the moles are too). There is no user input, so its more of a simulation displayed in a GUI. It creates n producer threads that create moles and place them into one shared bounded queue, along with n consumer threads that remove and whack the moles. Since the threads run concurrently and each mole has different delays, the gui appears less sequential and a bit more random.

