# -*- coding: utf-8 -*-
"""
Created on Wed Sep 14 13:08:36 2016

@author: xt
"""

def generate (num,i):
    if(i == 0):
        return num
    else:
        return (generate(num,i-1)**2)%10000

def lst (num, i):
    lst = []
    for n in range(i):
        lst += [generate(num,n)]
    return lst