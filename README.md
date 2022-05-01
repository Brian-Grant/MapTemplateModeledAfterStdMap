Brian Grant
Spring 2021
540
Assignment 02

run make 
	-to build the tests provided

run make runtests
	-to run provided tests minus the interactive morse code exec
	-the output of the scaling test is redirectedd to scaling.txt

run make clean 
	-to clean up the directory

run make cleantxt
	-to delete output files

everything else in the directory is related to tests I implemented
	-as of now myTest does not compile, because I got good use out of it
	and decided not to continue
	-compilation of this is not needed


The templates are in the directory Template
Provided tests are in directory tests



I have left in a decent amount of commented out code because
I wanted to approach this assignment with a "test driven" or more 
accurately "debug driven" development mindset. 
Have a look if you are interested.

In terms of the time complexity
What I observe is that there is a decent amount of randomness to the 
	performance.
This is most likely expected due to the random element of the skiplist,
as well as the randomness within the scaling test executable.

Sometimes my map performs better
Sometimes my map performs worse
Sometimes my map performs much better
Sometimes my map performs much worse

I have run the code to convince myself that on average after many
program runs, the time complexity of the map implemented here is
decently comparable to the time complexity of the std map

It is definitely not as streamlined as the std map implementation
but overall I am content with the resulting time complexity


Extra Credit: I did not do the extra credit
