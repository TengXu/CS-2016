# BEGIN binary_search.py

# 5 September 2016
# this module (i.e. script file) 'binary_search.py' contains one function
# definition 'bin_search' which takes two arguments:
#    'arr', which must be a sorted array with all elements of the same type T,
#    'elem', which is an element of type T
# and returns the index of 'elem' in 'arr'. If 'elem' occurs more than once
# in 'arr', then 'bin_search' returns one of the indeces of 'elem' in 'arr'.

# issue command execfile ('binary_search.py') before using bin_search

# side remark: 'module' is not a keyword in Python as it is in Haskell

# a Python module should have a short, all-lowercase name, although
# underscores can be used if it improves readability. It can contain
# classes, functions, and global variables.

# a Python function has the same naming convention as a Python module.

# a Python package (i.e. a directory of Python modules) should also have a
# short, all-lowercase names, with the use of underscores being discouraged.

# a Python class should be named using the CapWords convention.

def bin_search (arr, elem) :
    def recurse (first, last) :
        mid = (first + last) / 2
        if first > last :
            print(elem, "is not in the array")
        elif (arr [mid] < elem) :
            return recurse (mid + 1, last)
        elif (arr [mid] > eleym) :
            return recurse(first, mid - 1)
        else :
            return mid

    return recurse (0, len(arr)-1)

# END binary_search.py
