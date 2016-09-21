# -*- coding: utf-8 -*-
"""
Created on Wed Sep 14 13:33:17 2016

@author: xt
"""

def prime (n):
    lst = []
    for x in range(2,n+1):
        lst += [x]
    return set([lst[0]] + [x for x in lst if x%lst[0] > 0])
    
