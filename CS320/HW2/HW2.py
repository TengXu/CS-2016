# -*- coding: utf-8 -*-
"""
Created on Thu Sep 29 22:15:20 2016

@author: xt
"""
#!/usr/bin/env python3.5
import sys
# Teng Xu xt@bu.edu

# Contrast between python and haskell:
# 1. Python does not have infinite list, so we need to use generator
# to creat infinite lists, which takes more steps than haskell
# 2. Haskell is more flexible of defining a function using pattern matching
# 3. Python is easier to test and detect errors and you can print elements 
# during the execution easily, but in Haskell it is more complicated

def prefix(n, x):
    if(n == 0):
        return []
    elif(x == []):
        return x
    else:
        return [x[0]] + prefix((n-1), x[1:])

def suffix(n, x):
    if(n == 0):
        return x
    elif(x == []):
        return x
    else:
        return suffix((n-1), x[1:])

def split(n,a,x):
    if(suffix(n,x))==[]:
        return x
#    elif(x == []):
#        return []
    else:
        return prefix(n,x) + [a] + split(n,a,suffix(n,x))

def plane(r):
    a = range((-r),r+1)
    b = range((-2*r),r+1)
    return [(x/(r*1.0),y/(r*1.0)) for y in a for x in b]

def p(a,b):
    return (b[0]*b[0]-b[1]*b[1]+a[0],2*b[0]*b[1]+a[1])

def orbit(a):
    lst = (0,0)
    while True:
        yield lst
        lst = p(a,(lst[0],lst[1]))

def take(xs, n):
    if n == 0:
        return [next(xs)]
    else:
        next(xs)
        return take(xs, n-1)
   
def disp(d,lst):
    if(lst == []):
        return ' '
    elif(d < lst[0][0]):
        return lst[0][1]
    else:
        return disp(d,lst[1:])
    
def norm(x,y):
    return x*x + y*y

def mandelbrot(r,i):
    q = plane(r)
    b = []
    for x in q:
        b += take(orbit(x),i)
    d = [norm(c[0],c[1]) for c in b]
    #print(d)
    f = [disp(e,[(0.15, '#'), (0.5, 'x'), (100, '.')]) for e in d]
    result = split((3*r+1),'\n',f)
    return result

#Python version
a = mandelbrot(20,20)
b = ''.join(a)
print(b)

# Command Line version
#print(sys.argv)
#x = int(sys.argv[1])
#y = int(sys.argv[2])
#z = mandelbrot(x,y)
#o = ''.join(z)
#print(o)
    