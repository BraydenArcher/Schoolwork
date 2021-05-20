I made the calculator data safe by using a mutex lock and unlock whenever my calc object assigned a variable.
It was quite easy to determine which sections of code were critical because when I originally wrote the program,
I broke it into chunks, with a simple assignment (a = 10) and a complex assignment
(a = a + 5). Whenever there is a simple of complex assignment that is requested,
I simply used a mutex and locked and unlocked immediately before and after the
assignment. This guaranteed that I am never modifying any data without using a mutex and
therefore will never have synchronization issues.

I also had a function for getting the value of a variable from my data structure. Simply putting a lock into this function
took care of every part of my code that requested a value from the data structure.