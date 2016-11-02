# -*- coding: utf-8 -*-
"""
Created on Mon Oct 31 14:53:55 2016

@author: xt
"""

# CS235 HW7
# Teng Xu xt@bu.edu

#3
def encryptVig(message, keyword):
    letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    result = ''
    mlen = len(message)
    length = len(keyword)
    i = 0
    while(i < mlen):
        for x in range(min(len(message),length)):
            a = letter.index(message[x])
            b = letter.index(keyword[x])
            c = (a+b) % 26
            result += letter[c]
            i += 1
        message = message[length:]
    return result
            
def decryptVig(message, keyword):
    letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    result = ''
    mlen = len(message)
    length = len(keyword)
    i = 0
    while(i < mlen):
        for x in range(min(len(message),length)):
            a = letter.index(message[x])
            b = letter.index(keyword[x])
            c = (a-b) % 26
            result += letter[c]
            i += 1
        message = message[length:]
    return result
    
def encryptTrit(message):
    letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    result = ''
    mlen = len(message)
    for x in range(mlen):
        a = letter.index(message[x])
        c = (a + x + 1) % 26
        result += letter[c]
    return result
    
def decryptTrit(message):
    letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    result = ''
    mlen = len(message)
    for x in range(mlen):
        a = letter.index(message[x])
        c = (a - x - 1) % 26
        result += letter[c]
    return result