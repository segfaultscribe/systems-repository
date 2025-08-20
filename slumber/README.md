# sleep() using nanosleep()

Pretty straightforward and small af. 

By using `timespec` struct, create a safe wrapper around `nanosleep()` to implement sleep in seconds.