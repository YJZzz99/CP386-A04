CP386 Assignment 4 
==================
Section C, Spring 2022

Makefile
========
#Do not edit the contents of this file.
CC = gcc
CFLAGS = -std=gnu99
TARGET = allocation  
OBJFILES = allocation.o
all: $(TARGET)

allocation: allocation.c
	$(CC) $(CFLAGS) -o allocation allocation.c
	
run: allocation
	./allocation 1048576
clean:
	rm -f $(OBJFILES) $(TARGET) *~ 
  
 Output
 ======
<img width="736" alt="image" src="https://user-images.githubusercontent.com/84728523/180583992-43e5a589-909e-49b0-a16b-58936f236606.png">
<img width="617" alt="image" src="https://user-images.githubusercontent.com/84728523/180584008-7cc969a9-f4d0-441d-ac75-ca0462c7b374.png">

Individual contribution
=======================
Mengyuan Liu  
Working for ：Request memory,Display process, and Release memory  
Huiqiao Yang
Working for ：First function,Best function, and Worst function  

Feature
====
RQ -> Request memory  
Status -> Display process  
RL -> Release memory  
F -> First function  
B -> Best function  
W -> Worst function

About developer
======

License
=======

