----------------------------------------------------------------
-- Computer Science 320 (Fall, 2016)
-- Concepts of Programming Languages
--
-- Assignment 6
--   Exp.hs

----------------------------------------------------------------
-- Abstract Syntax for the mini-Haskell Interpreter

module Exp (Oper(..), Exp(..), opsWithStrings, opsWithStrings2, noLets, subst)
  where

data Oper = Plus | Times
          | Equal
          | And | Or | Not
          | Head | Tail | Cons
  deriving Eq

data Exp = Unit | Nil
         | N Int | B Bool | Var String | Op Oper
         | App Exp Exp
         | LamUnit Exp
         | Lam String Exp
         | If Exp Exp Exp
         | Let [(String, Exp)] Exp

----------------------------------------------------------------
-- Printing functions for Abstract Syntax

ops       = [Not, Head, Tail,
             Plus, Times, Equal, And, Or, Cons]
opStrings = ["not", "head", "tail",
             "(+)", "(*)", "(==)", "(&&)", "(||)", "(:)"]
opsWithStrings = zip ops opStrings

showOper op = fOp op opsWithStrings
  where
    fOp op ((o,n):ons) = if (o == op) then n else (fOp op ons)
    fOp op []           = "impossible"

-- Convenient list for the parser.
ops2       = [Plus, Times, Equal, And, Or, Cons]
opStrings2 = ["+)", "*)", "==)", "&&)", "||)", ":)"]
opsWithStrings2 = zip ops2 opStrings2

showExp wh Unit  = wh ++ "()"
showExp wh Nil   = wh ++ "[]"
showExp wh (N n) = wh ++ (show n)
showExp wh (B b) = wh ++ (show b)
showExp wh (Var x) = wh ++ x
showExp wh (Op o) = wh ++ (show o)
showExp wh (App e1 e2) = "(" ++ (showExp wh e1) ++
                         " " ++ (showExp wh e2) ++ ")"
showExp wh (LamUnit e) = "\\() -> " ++ (showExp wh e)
showExp wh (Lam x e) = "\\" ++ x ++ " -> " ++ (showExp wh e)
showExp wh (If e1 e2 e3) = "if " ++ (showExp wh e1) ++
                           " then " ++ (showExp wh e2) ++
                           " else " ++ (showExp wh e3)
showExp wh (Let xses e) = "let\n" ++ (showBinds wh xses) ++
                          "in \n  " ++ (showExp wh e)

showBinds wh [] = "impossible"
showBinds wh [(n, e)] = "  " ++ n ++ " = " ++ (showExp wh e) ++ "\n"
showBinds wh ((n, e):xns) = "  " ++ n ++ " = " ++ (showExp wh e) ++
                                 ";\n" ++ (showBinds wh xns)

noLets::Exp -> Exp
noLets (LamUnit e) = LamUnit (noLets e)
noLets (Let [] e) = noLets e
noLets (Let ((x,exp):xs) e) = noLets (Let xs (App (Lam x e) (noLets exp)))
noLets (App e1 e2) = (App (noLets e1) (noLets e2))
noLets (If e1 e2 e3) = (If (noLets e1) (noLets e2) (noLets e3))
noLets e = e

subst::String -> Exp -> Exp -> Exp
subst x n (App e1 e2) = (App (subst x n e1) (subst x n e2))
subst x n (If e1 e2 e3) = (If (subst x n e1) (subst x n e2) (subst x n e3))
subst x n (LamUnit e) = (LamUnit (subst x n e))
subst x n (Lam s e)
          | x == s = (Lam s e)
          | otherwise = Lam s (subst x n e)
subst x n (Var x')
        | x == x' = n
        | otherwise = (Var x')
subst _ _ e = e

instance Show Oper where
  show = showOper

instance Show Exp where
  show = showExp []
