-- BEGIN SelectRandom.hs

-- 22 august 2016
-- module to produce n unique random integers from an interval [x,y] of ints
--
-- execute from interpreter command line with ':load SelectRandom', after
-- which the following are examples of how to use it:
--    command 'select 7 [10..150] (mkStdGen 42)' or also
--    command 'fst (select 7 [10..150] (mkStdGen 42))' 
-- should return a list of 7 randomly selected entries from the list [10..150]

module SelectRandom (select) -- use CapWords convention for naming Haskell modules
   where

-- preceding two lines can be omitted, but it is good practice to wrap 
-- all the methods (here only one, 'select') in this file in the same 
-- module (here 'SelectRandom')

import System.Random  -- this library module includes several functions, types,
                      -- and typeclasses, related to random-number generation.
                      -- In some older interpreters/compilers, you may write
                      -- 'import Random' instead of 'import System.Random'

select :: Int -> [a] -> StdGen -> ([a], StdGen)

-- 'select' selects n random items from list 'lst' without repetitions

select n lst g = help n lst g (length lst - 1) []
  where  help 0 _ g _ ps     = (ps, g)
         help n lst g max ps =  help (n-1) (left ++ right) g' (max-1) (pick : ps)
           where (i, g') = randomR (0, max) g
                 (left, pick : right) = splitAt i lst

-- END SelectRandom.hs
