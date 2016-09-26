# BEGIN fib_sequence.py

# 5 September 2016
# this module (i.e. script file) 'fib_sequence.py' contains a function definition
# 'recursive_fib' which takes one argument n and returns the n-th Fibonacci number 
# (with the 0-th being the first). 

# issue command "execfile ('fib_sequence.py')" -- without the outer
# double quotes -- before using 'recursive_fib'

# side remark: 'module' is not a keyword in Python as it is in Haskell

# a Python module should have a short, all-lowercase name, although
# underscores can be used if it improves readability. It can contain
# classes, functions, and global variables.

# a Python package (i.e. a directory of Python modules) should also have a
# short, all-lowercase names, with the use of underscores being discouraged.

# a Python class should be named using the CapWords convention.

# this module/script file contains a Python program to display the Fibonacci 
# sequence up to n-th term using recursive functions.

def recursive_fib (n):
   """Recursive function to
   print Fibonacci sequence"""
   if n <= 1:
       return n
   else:
       return (recursive_fib (n-1) + recursive_fib (n-2))

# take input from the user
nterms = int (input("How many terms? "))

# check if the number of terms is valid
if nterms <= 0:
   print ("Plese enter a positive integer")
else:
   print ("Fibonacci sequence:")
   for i in range (nterms):
       print(recursive_fib (i))

# END fib_sequence.py
