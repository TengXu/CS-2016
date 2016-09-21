# -*- coding: utf-8 -*-
"""
Created on Wed Sep 14 22:38:19 2016

@author: xt
"""

def checkAll(s,p):
    for x in list(s):
        if not p(x):
            return False
    return True
def check(x):
    return checkAll(x, lambda x: x>999 and x<3000)
    
    
def remove(n):
    n = list(n)
    if n == []:
        return []
    else:
        if(n[0]>999 and n[0]<3000):
            return [n[0]]+remove(n[1:])
        else:
            return remove(n[1:])
def removeset(n):        # from list to set
    return set(remove(n))
    
    
def leap(n):
    n = list(n)
    if n == []:
        return []
    else:
        if((n[0]%4 ==0 and n[0]%100 !=0) or n[0]%400 ==0):
            return [n[0]]+leap(n[1:])
        else:
            return leap(n[1:])
def leapyear(n):        # from list to set
    return set(leap(n))

