# Thread Comparison
This assignment is written in C to run on a Linux server. It is not guaranteed to work on Windows or macOS.
This assignment centered around comparing different methods of doing the same thing, one serial and two thread based. One thread based implementation was using a mutex lock which caused
each thread to wait on each other often, the other used local histograms so that only when updating at the end could there be a wait when merging the 3 local histograms into the main one.
I received full marks on this assignment. 