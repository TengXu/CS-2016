----------------------------------------------------------------
-- Computer Science 320 (Fall, 2016)
-- Concepts of Programming Languages
--
-- Assignment 6
--   Val.hs

----------------------------------------------------------------
-- Value Representation for mini-Haskell

module Val (Val(..)) where

import Exp (Exp(..), Oper(..))
import Env

data Val = VUnit | VNil
         | VN Int | VB Bool | VOp Oper
         | Partial Oper Val
         | VLamUnit Exp (Env Val)
         | VLam String Exp (Env Val)
         | VListBool [Bool]
         | VListInt [Int]

----------------------------------------------------------------
-- Show Function for Values

showVal VUnit = "()"
showVal VNil = "[]"
showVal (VN n) = show n
showVal (VB b) = show b
showVal (VOp op) = show op
showVal (Partial op v) = "(" ++ (show op)++" "++(show v) ++ ")"
showVal (VLamUnit e env) = "\\() -> " ++ (show e)
showVal (VLam n e env) = "\\" ++ n ++ " -> " ++ (show e)
showVal (VListBool bs) = show bs
showVal (VListInt ns) = show ns

instance Show Val where
  show = showVal
