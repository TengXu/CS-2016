-- Teng Xu (xt@bu.edu)

tet :: Int -> Int
tet n
    | n == 0 = 0
    | n == 1 = 1
    | n == 2 = 1
    | n == 3 = 2
    | otherwise = tet (n-1) + tet (n-2) + tet (n-3) + tet (n-4)

-- the number of times the function will be called on
-- an input of size n£º 4^n
