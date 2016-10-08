# -*- coding: utf-8 -*-
"""
Created on Sat Oct  8 15:10:50 2016

@author: xt
"""

# Teng Xu xt@bu.edu
# HW4
import string

def checkUPC(a):
    Sum1 = int(a[0])*3 + int(a[2])*3 + int(a[4])*3 + int(a[6])*3 + int(a[8])*3 + int(a[10])*3
    Sum2 = int(a[1]) + int(a[3]) + int(a[5]) + int(a[7]) + int(a[9]) + int(a[11])
    if((Sum1 + Sum2)%10 == 0):
        return True
    else:
        return False
        
def digits_of(number):
    return list(map(int, str(number)))
    
def luhn_checksum(card_number):
    digits = digits_of(card_number)
    odd_digits = digits[-1::-2]
    even_digits = digits[-2::-2]
    total = sum(odd_digits)
    for digit in even_digits:
        total += sum(digits_of(2*digit))
    return total % 10
    
def is_luhn_valid(card_number):
    return luhn_checksum(card_number) == 0
    
# First compute the sum of the first 10 digits, and return 10 - (sum mod 10)
def addCheckDigitUPC(a):
    Sum = int(a[0])*3 + int(a[2])*3 + int(a[4])*3 + int(a[6])*3 + int(a[8])*3 + int(a[10])*3
    Sum += int(a[1]) + int(a[3]) + int(a[5]) + int(a[7]) + int(a[9])
    return 10 - (Sum%10)
    
# First compute the sum of the first 11 digits, and return 10 - (sum mod 10)
def addCheckDigitCC(a):
    digits = digits_of(a)
    odd_digits = digits[-2::-2]
    even_digits = digits[-1::-2]
    total = sum(odd_digits)
    for digit in even_digits:
        total += sum(digits_of(2*digit))
    return 10 - (total%10)
    
def encryptShiftCipher(message, key):
    letter = "abcdefghijklmnopqrstuvwxyz "
    number = []
    newletter = ""
    for i in range(len(message)):
        if (message[i].lower()) not in letter:
            number += []
        else:
            number += [letter.index(message[i].lower())]    
    for x in number:
        index = x+key
        if index > 25:
            index -= 26
        if x == 26:
            index = 26
        newletter += letter[index]
    return newletter
    
def decryptShiftCipher(message, key):
    letter = "abcdefghijklmnopqrstuvwxyz "
    number = []
    newletter = ""
    for i in range(len(message)):
        if (message[i].lower()) not in letter:
            number += []
        else:
            number += [letter.index(message[i].lower())]    
    for x in number:
        index = x-key
        if index < 0 :
            index += 26
        if x == 26:
            index = 26
        newletter += letter[index]
    return newletter
    