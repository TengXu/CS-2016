----------------------------------------------------------------
-- Computer Science 320 (Fall, 2016)
-- Concepts of Programming Languages
--
-- Assignment 6
--   Eval.hs
-- Teng Xu

----------------------------------------------------------------
-- Evaluation Functions for the mini-Haskell Interpreter

module Eval (evalExp) where

import Env
import Err (Error(..))
import Exp (Oper(..), Exp(..))
import Val (Val(..))

----------------------------------------------------------------
-- This function is exported to the Main module.

evalExp :: Exp -> Error Val
evalExp e = ev e emptyEnv

----------------------------------------------------------------
-- Functions for evaluating operations applied to values.

appOp :: Oper -> Val -> Error Val
appOp Plus (VN a) = S (Partial Plus (VN a))
appOp Times (VN a) = S (Partial Times (VN a))
appOp Equal (VN a) = S (Partial Equal (VN a))
appOp Equal (VB a) = S (Partial Equal (VB a))
appOp And (VB a) = S (Partial And (VB a))
appOp Or (VB a) = S (Partial Or (VB a))
appOp Cons (VN a) = S (Partial Cons (VN a))
appOp Cons (VB a) = S (Partial Cons (VB a))
appOp Cons (VListInt a) = S (Partial Cons (VListInt a))
appOp Cons (VListBool a) = S (Partial Cons (VListBool a))
appOp Not (VB a) = S (VB (not a))
appOp Head (VListBool (x:xs)) = S (VB x)
appOp Head (VListInt (x:xs)) = S (VN x)
appOp Tail (VListBool (x:xs)) = S (VListBool xs)
appOp Tail (VListInt (x:xs)) = S (VListInt xs)
appOp _ _ = Error "Error"

appBinOp :: Oper -> Val -> Val -> Error Val
appBinOp Plus (VN a) (VN b) = S (VN (a + b))
appBinOp Times (VN a) (VN b) = S (VN (a * b))
appBinOp Equal (VN a) (VN b) = S (VB (a == b))
appBinOp Equal (VB a) (VB b) = S (VB (a == b))
appBinOp Equal (VListInt a) (VListInt b) = S (VB (a == b))
appBinOp Equal (VListBool a) (VListBool b) = S (VB (a == b))
appBinOp And (VB a) (VB b) = S (VB (a && b))
appBinOp Or (VB a) (VB b) = S (VB (a || b))
appBinOp Cons (VN a) (VListInt b) = S (VListInt (a:b))
appBinOp Cons (VB a) (VListBool b) = S (VListBool (a:b))
appBinOp Cons (VN a) (VNil) = S (VListInt ([a]))
appBinOp Cons (VB a) (VNil) = S (VListBool ([a]))
appBinOp _ _ _ = Error "Error"

----------------------------------------------------------------
-- Function for applying one value to another.

appVals :: Val -> Val -> Error Val
appVals (VOp a) b = appOp a b
appVals (Partial a b) c = appBinOp a b c
appVals (VLam x xs env) e = ev xs (updEnv x e env)
appVals (VLamUnit xs env) VUnit = ev xs env
appVals _ _ = Error "Error"

----------------------------------------------------------------
-- Function for evaluating an expression with no bindings or
-- variables to a value.

ev0 :: Exp -> Error Val
ev0 Unit = S (VUnit)
ev0 Nil = S (VNil)
ev0 (N n) = S (VN n)
ev0 (B b) = S (VB b)
ev0 (Op o) = S (VOp o)
ev0 (App e1 e2) = case ev0 e1 of
    Error _ -> Error "Error"
    S v1 -> case ev0 e2 of
        Error _ -> Error "Error"
        S v2 -> appVals v1 v2
ev0 (If e1 e2 e3) = case ev0 e1 of
    Error _ -> Error "Error"
    S (VB True) -> ev0 e2
    S (VB False) -> ev0 e3
ev0 _ = Error "Error"

---------------------------------------------------------------
-- Function for evaluating an expression to a value. Note the
-- need for an environment to keep track of variables.

ev :: Exp -> Env Val -> Error Val
ev Unit env = S (VUnit)
ev Nil env = S (VNil)
ev (N n) env = S (VN n)
ev (B b) env = S (VB b)
ev (Op o) env = S (VOp o)
ev (App e1 e2) env = case ev e1 env of
    Error _ -> Error "Error"
    S v1 -> case ev e2 env of
        Error _ -> Error "Error"
        S v2 -> appVals v1 v2
ev (If e1 e2 e3) env = case ev e1 env of
    Error _ -> Error "Error"
    S (VB True) -> ev e2 env
    S (VB False) -> ev e3 env
ev (Var x) env = case findEnv x env of
        Nothing -> Error "Error"
        Just a -> S a
ev (LamUnit e) env = S (VLamUnit e env)
ev (Lam x e) env = S (VLam x e env)
ev (Let [] e) env = ev e env
ev (Let (x:xs) e) env = case ev (snd x) env of
                Error _ -> ev (Let xs e) env
                (S v) -> ev (Let xs e) (updEnv (fst x) v env)
ev _ _ = Error "Error"



