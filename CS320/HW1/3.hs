-- Teng Xu (xt@bu.edu)
-- 3.1
tet :: [Int] -> [Int]
tet(x) = sum (take 4 x) : x

--3.2
recur :: Int -> [Int]
recur 0 = [0]
recur n = recur(n-1) ++ [n]

--3.3
tet2help :: Int -> [Int] -> Int
tet2help n lst
    |n == 0 = 0
    |n == 1 = 1
    |n == 2 = 1
    |n == 3 = 2
    |n+1 == length lst = head lst
    |otherwise = tet2help n (tet lst)

tet2 :: Int -> Int
tet2 n = tet2help n [2,1,1,0]
