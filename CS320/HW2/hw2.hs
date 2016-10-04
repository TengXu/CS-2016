-- Teng Xu xt@bu.edu
-- HW2


-- 1
--prefix :: Int -> [a] -> [a]
prefix _ [] = []
prefix 0 _ = []
prefix n (x:xs) = x:(prefix (n-1) xs)

--suffix :: Int -> [a] -> [a]
suffix _ [] = []
suffix 0 xs = xs
suffix n (x:xs) = suffix (n-1) xs


--split :: Int -> a -> [a] -> [a]
split n a xs
    | xs == [] = []
    | len xs < n = xs
    | otherwise = (prefix n xs) ++ [a] ++ (split n a (suffix n xs))

--len :: [a] -> Int
len [] = 0
len (x:xs) = 1 + len xs

-- 2
plane r = [(x/r,y/r) | y <-[-r*1.0..r*1.00], x <-[-2.0*r..r]]

p (x,y) (u,v) = (u*u-v*v+x, 2*u*v+y)

orbit a = (0,0):[p a (fst i, snd i) | i <- (orbit a)]

disp d lst
    | lst == [] = ' '
    | d < (fst (lst!!0)) = (snd (lst!!0))
    | otherwise = disp d (tail lst)

norm (x,y) = x*x + y*y

mandelbrot r i l =split (r*3+1) '\n' ([disp c l|c <- [norm (fst b, snd b)| b <- [(orbit q)!!i|q <- (plane r)]]])






