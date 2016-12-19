----------------------------------------------------------------
-- Computer Science 320 (Fall, 2016)
-- Concepts of Programming Languages
--
-- Assignment 6
--   Eval.hs

----------------------------------------------------------------
-- Evaluation for mini-Haskell

module Eval (evalExp) where

import Env
import Err
import Exp
import Val

----------------------------------------------------------------
-- This function is exported to the Main module.

-- Problem 3, Part C
evalExp :: Exp -> Error Val
evalExp e = ev (noLets e) emptyEnv

----------------------------------------------------------------
-- Functions for evaluating operations applied to values.

-- Problem 1, Part A
appOp :: Oper -> Val -> Error Val
appOp Not  (VB b) = S (VB (not b))
appOp Head (VListBool (b:bs)) = S (VB b)
appOp Head (VListInt  (n:ns)) = S (VN n)
appOp Tail (VListBool (b:bs)) = S (VListBool bs)
appOp Tail (VListInt  (n:ns)) = S (VListInt  ns)
appOp Head _ = Error "head applied to an empty list"
appOp Tail _ = Error "tail applied to an empty list"
appOp Not  _ = Error "not applied to non-boolean"
appOp op v2 = S (Partial op v2)

-- Problem 1, Part B
appBinOp :: Oper -> Val -> Val -> Error Val
appBinOp Plus  (VN n) (VN n') = S $ VN (n + n')
appBinOp Times (VN n) (VN n') = S $ VN (n * n')
appBinOp Equal (VN n) (VN n') = S $ VB (n == n')
appBinOp Equal (VB b) (VB b') = S $ VB (b == b')
appBinOp And   (VB b) (VB b') = S $ VB (b && b')
appBinOp Or    (VB b) (VB b') = S $ VB (b || b')
appBinOp Cons  (VB b) (VListBool bs) = S $ VListBool (b:bs)
appBinOp Cons  (VN n) (VListInt ns)  = S $ VListInt (n:ns)
appBinOp Cons  (VB b) VNil           = S $ VListBool (b:[])
appBinOp Cons  (VN n) VNil           = S $ VListInt (n:[])
appBinOp op v v' =
  Error $ "binary operator "
           ++ (show op)
           ++ "not defined on arguments "
           ++ (show v) ++ " and " ++ (show v')

----------------------------------------------------------------
-- Function for applying one value to another.

-- Problem 1, Part C; Problem 3, Part A
appVals :: Val -> Val -> Error Val
appVals (VOp op)         v2     = appOp op v2
appVals (Partial op v1 ) v2     = appBinOp op v1 v2
appVals (VLamUnit e env) VUnit  = ev e env
appVals (VLam x e env)   v2     = ev e (updEnv x v2 env)
appVals v1 v2 = Error $ (show v1)
                        ++ " cannot be applied to " ++ (show v2)

appValExp::Val -> Exp -> Error Val
appValExp (VLamUnit e env) exp = ev e env
appValExp (Partial And (VB False)) exp = S (VB False)
appValExp (Partial Or (VB True)) exp = S (VB True)
appValExp (VLam str e env) exp = ev (subst str exp e) env
appValExp a e = case (ev0 e) of
                Error _ -> Error "Error"
                S v -> (appVals a v)

----------------------------------------------------------------
-- Function for evaluating an expression with no bindings or
-- variables to a value.

-- Problem 1, Part D
ev0 :: Exp -> Error Val
ev0 Unit    = S VUnit
ev0 Nil     = S VNil
ev0 (N n)   = S (VN n)
ev0 (B b)   = S (VB b)
ev0 (Op op) = S (VOp op)

ev0 (App e1 e2) =
  case (ev0 e1) of
    Error err -> Error err
    S v1 -> (appValExp v1 e2)

ev0 (If e1 e2 e3) =
  case (ev0 e1) of
    S (VB c)  -> if c then ev0 e2 else ev0 e3
    S _       -> Error "'if' condition not a boolean"
    Error err -> Error err

ev0 (LamUnit e) = S (VLamUnit e emptyEnv)
ev0 (Lam x e) = S (VLam x e emptyEnv)
ev0 _ = Error "expression too complex for ev0"

----------------------------------------------------------------
-- Function for evaluating an expression to a value. Note the
-- need for an environment to keep track of variables.

-- Problem 3, Part B
ev :: Exp -> Env Val -> Error Val
ev Unit    env = S VUnit
ev Nil     env = S VNil
ev (N n)   env = S (VN n)
ev (B b)   env = S (VB b)
ev (Op op) env = S (VOp op)

ev (Var x) env =
  case (findEnv x env) of
    Just x' -> S x'
    Nothing -> Error $ "unbound variable: " ++ x

ev (App (Lam str e1) e2) env = ev (subst str (App (LamUnit e2) Unit) e1) (updEnv str (VLamUnit e2 emptyEnv) env)

--ev (App e1 e2) env =
--  case (ev e1 env) of
--    Error err -> Error err
--    S v1 -> (appValExp v1 e2)

-- Notice in these cases how we store the current
-- environment inside the closure. If the closure is
-- applied to an argument at any other point, this
-- environment can then be used to evaluate
-- the body of the lambda abstraction.
ev (Lam x e)   env = S (VLam x e env)
ev (LamUnit e) env = S (VLamUnit e env)

ev (If e1 e2 e3) env =
  case (ev e1 env) of
    S (VB c)  -> if c then ev e2 env else ev e3 env
    S _       -> Error "'if' condition not a boolean"
    Error err -> Error err
ev a env = ev0 a

-- There are two cases here, the first occurs if the
-- variable being bound does not occur in its own
-- definition. In the second case, we must abstract
-- over the variable and apply 'fix' to this abstraction,
-- as specified in the evaluation rules.




-- Helper value, this is the lambda expression which is
-- equivalent to the 'fix' operator, and can be used with
-- call-by-value evaluation (also known as the "Z"
-- combinator, as opposed to the "Y" combinator).
fixExp = Lam "f"
         (App (Lam "x" (App (Var "f") (Lam "y"
                  (App (App (Var "x") (Var "x")) (Var "y")))))
              (Lam "x" (App (Var "f") (Lam "y"
                  (App (App (Var "x") (Var "x")) (Var "y"))))))

-- Helper function to check whether a variable 'x' is free
-- in an expression.
fv :: String -> Exp -> Bool
fv x (Var x')      = x == x'
fv x (Lam x' e)    = if (x == x') then False else fv x e
fv x (LamUnit e)   = fv x e
fv x (App e1 e2)   = (fv x e1) || (fv x e2)
fv x (If e1 e2 e3) = (fv x e1) || (fv x e2) || (fv x e3)
fv x (Let [] be)   = fv x be
fv x (Let ((x',e):nes) be) =
  if (x == x') then False else (fv x (Let nes be)) || (fv x e)
fv x _        = False
