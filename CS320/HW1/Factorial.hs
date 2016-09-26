-- BEGIN Factorial.hs

-- 16 august 2016
-- execute by issuing command ':load Factorial' (without single quotes) 

module Factorial (fact) -- use CapWords convention for naming Haskell modules
   where

-- preceding two lines can be omitted, but it is good practice to wrap 
-- all the Haskell methods (here only one, 'fact') in the module to 
-- which they belong (here 'Factorial')

fact :: Int -> Int
fact n = prod (down n)

prod :: [Int] -> Int
prod [] = 1
prod (a:as) = a * prod as

down :: Int -> [Int]
down 0 = []
down n = n : down (n-1)

-- END Factorial.hs
