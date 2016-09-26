import random
def selectRan (n, lst):
    return select(n,lst[0],lst[-1],[])

# helper function
def select(n, a, b, lst):
    if n == 0:
        return lst
    else:
        n = random.randint(a, b)
        while(n in lst):
            n = random.randint(a, b)
        lst += [n]
        return select(n-1, a, b,lst)

