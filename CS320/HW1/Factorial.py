# -*- coding: utf-8 -*-
"""
Created on Sun Sep 11 22:33:25 2016

@author: xt
"""

def prod(a):
    if(a == []):
        return 1
    else:
        return a[0]*prod(a[1:])
        
def down(b):
    if(b == 0):
        return[]
    else:
        return[b]+down(b-1)
        
def fact(c):
    return prod(down(c))