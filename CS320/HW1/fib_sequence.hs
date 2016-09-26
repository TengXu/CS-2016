module Fib_sequence (fib)
   where

fib ::  Int -> Int
fib n
    | n <= 1 = n
    | otherwise = fib (n-1) + fib (n-2)

main = do
    putStrLn "How many terms? "
    nterms <- getLine
    let a = read nterms :: Int
    if a <= 0
    then putStrLn "Plese enter a positive integer"
    else do putStrLn "Fibonacci sequence:"
            mapM(\x -> print $ (fib x)) [0..(a-1)]
            return ()




