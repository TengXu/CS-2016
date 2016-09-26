bin :: [Int] -> Int -> Int
bin arr elem = recurse 0 (length arr -1) elem arr

recurse :: Int -> Int -> Int -> [Int] -> Int
recurse first last elem arr
    | first > last = -1
    | arr!!mid < elem = recurse (mid + 1) last elem arr
    | arr!!mid > elem = recurse first (mid - 1) elem arr
    | otherwise = mid
    where mid = (first + last) `div` 2
