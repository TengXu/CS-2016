----------------------------------------------------------------
-- Computer Science 320 (Fall, 2016)
-- Concepts of Programming Languages
--
-- Assignment 6
--   Env.hs
-- Teng Xu

----------------------------------------------------------------
-- Environments

module Env (
Env(..),
emptyEnv,
updEnv,
findEnv)
  where

type Env a = [(String, a)]

emptyEnv::Env a
emptyEnv = []

updEnv::String -> a -> Env a -> Env a
updEnv str a lst = (str, a):lst

findEnv::String -> Env a -> Maybe a
findEnv str lst = helper str lst

--Helper function
helper str [] = Nothing
helper str (x:xs)
            | (fst x) == str = Just (snd x)
            | otherwise = helper str xs
