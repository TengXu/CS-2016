# -*- coding: utf-8 -*-
"""
Created on Wed Sep 28 21:03:01 2016

@author: xt
"""

# Teng Xu xt@bu.edu
# HW3_2

def checkUPC(a):
    Sum1 = int(a[0])*3 + int(a[2])*3 + int(a[4])*3 + int(a[6])*3 + int(a[8])*3 + int(a[10])*3
    Sum2 = int(a[1]) + int(a[3]) + int(a[5]) + int(a[7]) + int(a[9]) + int(a[11])
    if((Sum1 + Sum2)%10 == 0):
        return True
    else:
        return False

def checkISBN10(a):
    if(a[9] == 'X'):
        Sum = int(a[0])*1 + int(a[1])*2 + int(a[2])*3 + int(a[3])*4 + int(a[4])*5 + int(a[5])*6
        Sum += int(a[6])*7 + int(a[7])*8 + int(a[8])*9 + 10*10 
    else:
        Sum = int(a[0])*1 + int(a[1])*2 + int(a[2])*3 + int(a[3])*4 + int(a[4])*5 + int(a[5])*6
        Sum += int(a[6])*7 + int(a[7])*8 + int(a[8])*9 + int(a[9])*10 
    if(Sum%11 == 0):
        return True
    else:
        return False