# -*- coding: utf-8 -*-
"""
Created on Sun Oct  9 17:12:32 2016

@author: xt
"""
# Teng Xu xt@bu.edu
# HW3

# Answer to 4(c):
# Advantages of higher-order functions:
# 1. Higher order function is applicable to many different problems.
# 2. Higher order function has a relativly fixed syntax so we will
# not be confused during writing the code and it also provides us with a
# idea or a hint on how to slove the problem.


class Tree:
    def __init__(self, a, left, right):
        self.value = a
        self.left = left
        self.right = right
    
    def __eq__(self,other):
        return self.List2() == other

    
    def leafCount(self):
        if(self.value == None):
            return 1
        elif(self.left != None) and (self.right != None):
            return self.left.leafCount() + self.right.leafCount()
        elif(self.left == None) and (self.right != None):
            return 1 + self.right.leafCount()
        elif(self.left != None) and (self.right == None):
            return 1 + self.left.leafCount()
        else:
            return 2
    
    def nodeCount(self):
        if(self.value == None):
            return 0
        elif(self.left != None) and (self.right != None):
            return 1 + self.left.nodeCount() + self.right.nodeCount()
        elif(self.left == None) and (self.right != None):
            return 1 + self.right.nodeCount()
        elif(self.left != None) and (self.right == None):
            return 1 + self.left.nodeCount()
        else:
            return 1
        
    def height(self):
        if(self.value == None):
            return 0
        elif(self.left != None) and (self.right != None):
            maxvalue = max(self.left.height(), self.right.height())
            return 1 + maxvalue
        elif(self.left == None) and (self.right != None):
            return 1 + self.right.nodeCount()
        elif(self.left != None) and (self.right == None):
            return 1 + self.left.nodeCount()
        else:
            return 1           
    
    def perfect(self):
        if(self.value == None):
            return True
        elif(self.left != None) and (self.right != None):
            if (self.left.height() == self.right.height()):
                return (self.left.perfect() and self.right.perfect())
        elif(self.left == None) and (self.right == None):
            return True
        else:
            return False
            
    def degenerate(self):
        if(self.value == None):
            return True   
        elif(self.left != None) and (self.right != None):
            return self.left.degenerate() and self.right.degenerate()
        elif(self.left == None) and (self.right != None):
            return self.right.degenerate()
        elif(self.left != None) and (self.right == None):
            return self.left.degenerate()
        else:
            return True
    
    def List(self):
        if(self.value == None or self == None):
            return []
        elif(self.degenerate == False):
            return []
        elif(self.left == None) and (self.right != None):
            return [self.value] + self.right.List()
        elif(self.left != None) and (self.right == None):
            return [self.value] + self.left.List()
        else:
            return [self.value]   
            
# get a list representation of the tree, helper function for equal 
    def List2(self):
        if(self.value == None):
            return [0,0,0]
        elif(self.left == None) and (self.right != None):
            return [self.value] + [0] + self.right.List2()
        elif(self.left != None) and (self.right == None):
            return [self.value] + self.left.List2() + [0]
        else:
            return [self.value] + [0] + [0]
        