----------------------------------------------------------------
-- Computer Science 320 (Fall, 2016)
-- Concepts of Programming Languages
--
-- Assignment 6
--   Env.hs

----------------------------------------------------------------
-- Environments

module Env (Env, emptyEnv, updEnv, findEnv)
  where

-- Problem 2, Part A
-- We represent environments as lists of association pairs,
-- where each pair indicates that the first component is the
-- variable associated with the second component.
type Env a = [(String, a)]

-- Problem 2, Part B
emptyEnv :: Env a
emptyEnv = []

-- Problem 2, Part C
-- When a new association is added, the old association
-- still exists in the list.
updEnv :: String -> a -> Env a -> Env a
updEnv n x e = (n, x):e

-- Problem 2, Part D
-- We simply search the list and return the value for the
-- first matching string.
findEnv :: String -> Env a -> Maybe a
findEnv n' ((n,x):nxs) =
  if n' == n then
    Just x
  else
    findEnv n' nxs
findEnv n' [] = Nothing
