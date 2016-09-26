# -*- coding: utf-8 -*-
"""
Created on Wed Sep 14 15:00:11 2016

@author: xt
"""
#3.1
def tet(lst):
    return [lst[0]+lst[1]+lst[2]+lst[3]]+lst

#3.2
def recur (n):
    if n == 0:
        return [0]
    else:
        return recur(n-1) + [n]

#3.3
def tet2(n):
    return tet2help(n, [2,1,1,0])

def tet2help(n, lst):
    if n == 0:
        return 0
    elif n == 1:
        return 1
    elif n == 2:
        return 1
    elif n == 3:
        return 2
    elif len(lst) == n+1:
        return lst[0]
    else:
        return tet2help(n, tet(lst))
