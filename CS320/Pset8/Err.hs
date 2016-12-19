----------------------------------------------------------------
-- Computer Science 320 (Fall, 2016)
-- Concepts of Programming Languages
--
-- Assignment 6
--   Err.hs

----------------------------------------------------------------
-- Error Representation for the mini-Haskell Interpreter

module Err (Error(..))
  where
  
data Error a = S a
             | Error String
             
instance Show a => Show (Error a) where
  show (Error s) = s
  show (S c)  = show c
