----------------------------------------------------------------
-- Computer Science 320 (Fall, 2012)
-- Concepts of Programming Languages
--
-- Assignment 8
--   Ty.hs

----------------------------------------------------------------
-- Syntax for Types for the mini-Haskell Interpreter

module Ty (typeCheck, AnnotVal(AnnotVal))
  where

import Exp (Exp(..), Oper(..))
import Err
import Env
import Val

data Ty = TyUnit
        | TyVar String
        | TyBool
        | TyInt
        | TyBoolList
        | TyIntList
        | Arrow Ty Ty
  deriving Eq

-- Annotated values, for printing in Main
data AnnotVal = AnnotVal Val Ty
instance Show AnnotVal where
  show (AnnotVal v t) = (show v) ++ " :: " ++ (show t)

-- Useful function for transforming environment (used for
-- applying substitutions to environment).
mapEnv :: (a -> b) -> Env a -> Env b
mapEnv s ((x, t):xts) = (x, s t):(mapEnv s xts)
mapEnv s [] = []

----------------------------------------------------------------
-- Canonical form

data PolyTy = ForAll [String] Ty
showVars (v:vs) = v ++ " "
showVars [v] = v
showVars [] = ""
instance Show PolyTy where
  show (ForAll [] t) = show t
  show (ForAll vs t) = "forall " ++ (showVars vs) ++ "." ++ (show t)

--canon :: Ty -> PolyTy
--Assignment 8, Problem #4 (b) Not Yet Implemented

--freevars :: Ty -> [String]
--Assignment 8, Problem #4 (a) Not Yet Implemented"]

----------------------------------------------------------------
-- This function is exported to the Main module.

-- For testing purposes, you may want to replace the
-- body of typeCheck with a call (ty0 e)

typeCheck :: Exp -> Error Ty
typeCheck e =
  case (ty emptyEnv freshTyVars e) of
    Error msg -> Error msg
    S (t, s, fv) -> S $ s t -- We apply the substitution before
                            -- returning the type.

----------------------------------------------------------------
-- Type-checking Algorithm

--data Ty = TyUnit
--        | TyVar String
--        | TyBool
--        | TyInt
--        | TyBoolList
--        | TyIntList
--        | Arrow Ty Ty

tyOp :: Oper -> Ty
tyOp Plus = Arrow (Arrow TyInt TyInt) TyInt
tyOp Times = Arrow (Arrow TyInt TyInt) TyInt
tyOp Equal = Arrow (Arrow TyInt TyInt) TyBool
tyOp And = Arrow (Arrow TyBool TyBool) TyBool
tyOp Or = Arrow (Arrow TyBool TyBool) TyBool
tyOp Not = Arrow (Arrow TyBool TyBool) TyBool
tyOp Head = Arrow TyIntList TyInt
tyOp Tail = Arrow TyIntList TyInt
tyOp Cons = Arrow (Arrow TyInt TyIntList) TyIntList
tyOp _ = TyVar "Error"

ty0 :: Exp -> Error Ty
ty0 Unit = S TyUnit
ty0 Nil = S (TyIntList)
ty0 (N n)   = S (TyInt)
ty0 (B b)   = S (TyBool)
ty0 (Op op) = S (tyOp op)
ty0 (If e1 e2 e3) =
    case (ty0 e1) of
    S (TyBool)  -> case (typeCheck e2) of
            S a -> case (typeCheck e3) of
                    S b -> if (a == b) then (ty0 e2) else Error "Error"
                    Error err -> Error err
            Error err -> Error err
    S _       -> Error "'if' condition not a boolean"
    Error err -> Error err
ty0 (App e1 e2) = case (typeCheck e1) of
    S (Arrow a b) -> case (typeCheck e2) of
           S c -> if (a == c) then S (Arrow a b) else Error "Error"
           Error err -> Error err
    d -> Error "Error"
ty0 (LamUnit e) = S (TyUnit)
ty0 _ = Error "Error"

ty :: Env Ty -> FreshVars -> Exp -> Error (Ty, Subst, FreshVars)

-- Base cases missing
-- Assignment 8, Problem #4 (a) Not Yet Implemented

ty gamma fvs (Var x) =
  case (findEnv x gamma) of
    Nothing -> Error $ "unbound variable " ++ x
    Just t  -> S (t, idsubst, fvs)

ty gamma fvs (If e1 e2 e3) =
  case (tys gamma fvs [e1, e2, e3]) of
    Error msg -> Error msg
    S ([t1, t2, t3], s, fvs') -> Error "Assignment 8, Problem #4 (b) Not Yet Implemented"

ty gamma fvs (App e1 e2) =
  case (tys gamma fvs [e1, e2]) of
    Error msg -> Error msg
    S ([t1, t2], s, (fv:fvs'')) ->
      case (unify (s t1) (Arrow (s t2) fv)) of
        Error msg -> Error msg
        S s'' -> S (s(s'' fv), s `o` s'', fvs'')

ty gamma (fv:fvs') (Lam x e) =
  Error "Assignment 8, Problem #4 (d) Not Yet Implemented"

ty gamma fvs (LamUnit e) =
  Error "Assignment 8, Problem #4 (c) Not Yet Implemented"

ty gamma (fv:fvs') (Let ((x,e):xes) be) =
  case (ty (updEnv x fv gamma) fvs' e) of
    Error msg       -> Error msg
    S (t, s, fvs'') -> ty (updEnv x (s t) gamma) fvs'' (Let xes be)
ty gamma fvs (Let [] be) = ty gamma fvs be

ty gamma fvs e = Error "cannot infer type"

-- This function infers the types of a list of expressions,
-- accumulating the substitutions and returning their
-- composition along with the list of types.
tys :: Env Ty -> FreshVars -> [Exp] -> Error ([Ty], Subst, FreshVars)
tys gamma fvs (e:es) =
  case (tys gamma fvs es) of
    Error msg -> Error msg
    S (ts, s, fvs') ->
      case (ty (mapEnv s gamma) fvs' e) of
        Error msg -> Error msg
        S (t, s', fvs'') -> S (t:ts, \x -> s (s' x), fvs'')
tys gamma fvs [] = S ([], idsubst, fvs)

----------------------------------------------------------------
-- Type Unification

unify :: Ty -> Ty -> Error Subst
unify _ _ = Error "Assignment 8, Problem #3 (a) Not Yet Implemented"

----------------------------------------------------------------
-- Type Variable Substitutions

type Subst = Ty -> Ty

placeHolder :: String -> Subst
placeHolder s = \t->TyVar s

idsubst :: Subst
idsubst = placeHolder "Assignment 8, Problem #2 (a) Not Yet Implemented"

o :: Subst -> Subst -> Subst
o _ _ = placeHolder "Assignment 8, Problem #2 (a) Not Yet Implemented"

subst :: String -> Ty -> Subst
subst _ _ = placeHolder "Assignment 8, Problem #2 (b) Not Yet Implemented"

----------------------------------------------------------------
-- Infinite List of Fresh Type Variables

type FreshVars = [Ty]
freshTyVars :: FreshVars
freshTyVars = [TyVar "Assignment 8, Problem #2 (c) Not Yet Implemented"]

----------------------------------------------------------------
-- Printing functions for Syntax of Types

showTy TyUnit      = "()"
showTy (TyVar s)   = s
showTy TyBool      = "Bool"
showTy TyInt       = "Int"
showTy TyBoolList  = "[Bool]"
showTy TyIntList   = "[Int]"
-- If the left argument of an arrow is an arrow, we need to
-- add parentheses to make sure the type is not ambiguous.
showTy (Arrow (Arrow t1 t2) t3) =
   "(" ++ (showTy (Arrow t1 t2)) ++ ") -> " ++ (showTy t3)
showTy (Arrow t1 t2) =
   (showTy t1) ++ " -> " ++ (showTy t2)

instance Show Ty where
  show = showTy
