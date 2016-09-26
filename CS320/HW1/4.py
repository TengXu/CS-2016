# 4.1
def recurfib (n, k):
    if k > len(n):
        return recurfib(n, len(n))
    elif k == 0:
        return 0
    else:
        return n[0] + recurfib(n[1:],k-1)

# 4.2
def recurfib2(n,k):
    return recurfib2help(n,k,[1,1,0])

def recurfib2help (n, k, lst):
    if(n == 0):
        return 1
    elif (n==1):
        return 1
    elif (n==2):
        return 1
    elif (n == len(lst)):
        return recurfib(lst,k)
    else:
        return recurfib2help(n,k,[recurfib(lst,k)]+lst)


