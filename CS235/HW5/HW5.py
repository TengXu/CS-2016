# -*- coding: utf-8 -*-
"""
Created on Fri Oct 14 00:41:10 2016

@author: xt
"""

# Teng Xu xt@bu.edu
# HW5

def gcd(a,b):
    if a > b:
        mod = a % b
        if mod == 0:
            return b
        else:
            return gcd(b,mod)
    else:
        mod = b % a
        if mod == 0:
            return a
        else:
            return gcd(a,mod)    
            
def f(e):
    return ((e*21)-63)%26
    