# -*- coding: utf-8 -*-
"""
Created on Wed Oct 19 15:06:17 2016

@author: xt
"""
# CS235 HW6
# Teng Xu xt@bu.edu

#2
def extendedGcd(a,b):
    q = [0]
    r = [a,b]
    s = [1,0]
    t = [0,1]
    i = 2
    if(a<b):
        c = a
        a = b
        b = c
        r = [a,b]
        x = s
        s = t
        t = x
    if(a%b == 0):
        r += [b]
        s += [1]
        t += [a//b]        
    else:
        while (a % b != 0):
            q += [a // b]
            r += [r[i-2] - r[i-1]*q[i-1]]
            s += [s[i-2] - s[i-1]*q[i-1]]
            t += [t[i-2] - t[i-1]*q[i-1]] 
            d = a%b
            a = b
            b = d
            i += 1
    # The first one is gcd, the second and third one are coefficients of
    # the first and second input
    return [r[-1],s[-1],t[-1]]
    
#3
# It is not possible for a multiplicative inverse modulo m to be 0, since 0 mod anything == 0
# Multiplicative inverse of a modulo m exists when gcd(a,m) == 1, when a and m are relatively prime
def multInverse(a,m):
    coeff = extendedGcd(a,m)[0]
    if(coeff == 1):
        return extendedGcd(a,m)[1]
    else:
        print("multiplicative inverse does not exist")
        
#4
def encryptAffine(l, a, b):
    if(gcd(a,26) != 1):
        return("error, gcd(a,26) != 1")
    else:
        string = ""
        for i in range(len(l)):
            if l[i] == ' ':
                string += ' '
            else:
                letter = "abcdefghijklmnopqrstuvwxyz"
                num = letter.index(l[i].lower())
                num = (a*num + b) % 26
                string += letter[num]
        return string

def decryptAffine(l, a, b):
    string = ""
    for i in range(len(l)):
        if l[i] == ' ':
            string += ' '
        else:
            letter = "abcdefghijklmnopqrstuvwxyz"
            num = letter.index(l[i].lower())
            num = ((num - b)*(extendedGcd(a,26)[1]))%26
            string += letter[num]
    return string
      
      
# helper to compute gcd
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
            
