-- Teng Xu (xt@bu.edu)
-- 4.1
recurfib :: [Int] -> Int -> Int
recurfib lst k
    | k > length lst = recurfib lst (length lst)
    | k == 0 = 0
    | otherwise = (head lst) + recurfib (tail lst) (k-1)

--4.2
recurfib2help :: Int -> Int -> [Int] -> Int
recurfib2help n k lst
    |n == 0 = 1
    |n == 1 = 1
    |n == 2 = 1
    |n == length lst = recurfib lst k
    |otherwise = recurfib2help n k ((recurfib lst k) : lst)

recurfib2 :: Int -> Int -> Int
recurfib2 n k = recurfib2help n k [1,1,0]
