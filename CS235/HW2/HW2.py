# -*- coding: utf-8 -*-
"""
Created on Fri Sep 23 12:29:04 2016

@author: xt
"""

# Teng Xu
# HW2

#1
def job(a,b):
    print(b.issubset(a))
    print(a.intersection(b))
    print(b.difference(a))
    print(a.difference(b))
    print(len(a.difference(b))/len(b))
    print(len(a.difference(b))/len(a))
    print(a.union(b))    
    
#2
def matching(a,b):
    print("number: ",len(product(a,b)))
    return product(a,b)

#3
def country(C,D):
    return quotient(C, D)
    
def minimumFlights(C, D):
    return len(quotient(C, D.union({(x, x) for x in C})))

#4
def hash(a):
    R = {(x,y) for x in a for y in a if x % 11 == y % 11}
    return quotient(a,R)
    
# helper function
def quotient(X,R):
    return {frozenset({y for y in X if (x,y) in R}) for x in X}
def product(X, Y):
    return { (x,y) for x in X for y in Y }    