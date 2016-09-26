# -*- coding: utf-8 -*-
"""
Created on Wed Sep 14 14:24:29 2016

@author: xt
"""
# 2
# Teng Xu (xt@bu.edu)
def tet (n):
    if n == 0:
        return 0
    elif n == 1:
        return 1
    elif n == 2:
        return 1
    elif n == 3:
        return 2
    else:
        return tet(n-1)+tet(n-2)+tet(n-3)+tet(n-4)

# the number of times the function will be called on
# an input of size n£º 4^n
