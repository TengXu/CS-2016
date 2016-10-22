-- CS320 HW4
-- Teng Xu

module Superstring
    where

import Data.List

type Str a = [a]

overlap :: Eq a => (Str a, Str a) -> Int
overlap (a, b)
        | (length a) == 0 = 0
        | (isPrefixOf a b) == True = length a
        | otherwise = overlap((tail a), b)

contains :: Eq a => Str a -> Str a -> Bool
contains a b
        | (length a) == 0 = False
        | (isPrefixOf b a) == True = True
        | otherwise = contains (tail a) b

o :: Eq a => Str a -> Str a -> Str a
a `o` b
    | (length a) == 0 = b
    | (isPrefixOf a b) == True = b
    | otherwise = [head a] ++ ((tail a) `o` b)

naive :: Eq a => [Str a] -> Str a
naive [] = []
naive a = foldr (o) [] a

maximize :: Eq a => (a -> Int) -> a -> a -> a
maximize f a b
        | f a > f b = a
        | otherwise = b

minimize :: Eq a => (a -> Int) -> a -> a -> a
minimize f a b
        | f a > f b = b
        | otherwise = a

update :: Eq a => [Str a] -> (Str a,Str a) -> [Str a]
update a (b, c) = (b`o`c) : filter contain a
                where
                contain x = not (contains (b`o`c) x)

allPairs :: Eq a => [Str a] -> [(Str a,Str a)]
allPairs a = [(x,y)|x <- a, y <- a, x /= y]

superstring :: Eq a => ([Str a] -> [(Str a,Str a)]) -> [Str a] -> Str a
superstring f [] = []
superstring f [a] = a
superstring f lst = foldr (minimize (length)) (naive lst) [superstring f y | y <-[update lst z | z <- (f lst)]]

optimal :: Eq a => [Str a] -> Str a
optimal a = superstring allPairs a


firstPair :: Eq a => [Str a] -> [(Str a,Str a)]
firstPair a = [((head a), (head(tail a)))]

-- helper for bestWithFirst
--largest x b str
--        | b == [] = str
--        | overlap (x, str) < (overlap (x, (head b))) = largest x (tail b) (head b)
--        | otherwise = largest x (tail b) str
--
--bestWithFirst :: Eq a => [Str a] -> [(Str a,Str a)]
--bestWithFirst a = [((head a), (largest (head a) (tail a) []))]
bestWithFirst_helper a lst str
	| length lst == 0 = str
	| overlap (a,str) < overlap (a,(head lst)) = bestWithFirst_helper a (tail lst) (head lst)
	| otherwise = bestWithFirst_helper a (tail lst) str

bestWithFirst :: Eq a => [Str a] -> [(Str a, Str a) ]
bestWithFirst a = [((head a), (bestWithFirst_helper (head a) (tail a) (head (tail a))))]

--my own superstring algorithm
bestWithEnd :: Eq a => [Str a] -> [(Str a, Str a) ]
bestWithEnd a = [((last a), (bestWithFirst_helper (last a) (init a) (last (init a))))]

-- helper for bestPair
largest2 a num pair
        | a == [] = pair
        | num < (overlap (head a)) = largest2 (tail a) (overlap (head a)) [(head a)]
        | otherwise = largest2 (tail a) num pair

bestPair :: Eq a => [Str a] -> [(Str a,Str a)]
bestPair a = largest2 (allPairs a) 0 (firstPair a)

greedy :: Eq a => [Str a] -> Str a
greedy a = superstring bestWithEnd a

compare :: Eq a => ([Str a] -> Str a) -> ([Str a] -> Str a) -> [Str a] -> Double
compare f1 f2 a = fromIntegral (length (f1 a)) / fromIntegral (length (f2 a))

-- 5(b)
-- bsetPair has the same result as optimal,
-- the other two are longer than optimal
--
-- firstPair vs optimal:
--compare greedy optimal test1
--1.51612903225806
--compare greedy optimal test2
--1.23529411764706
--compare greedy optimal test3
--1.25

--bestWithFirst vs optimal:
--compare greedy optimal test1
--1.25806451612903
--compare greedy optimal test2
--1.23529411764706
--compare greedy optimal test3
--1.125

--bestPair vs optimal:
--compare greedy optimal test1
--1.0
--compare greedy optimal test2
--1.0
--compare greedy optimal test3
--1.0

--my own bestWithEnd vs optimal:
--compare greedy optimal test1
--1.32258064516129
--compare greedy optimal test2
--1.0
--compare greedy optimal test3
--1.125

test1 = ["ctagcgacat", "aagatagtta", "gctactaaga", "gacatattgt", "tagttactag"]
test2 = ["101001","010100010100", "100101", "001010", "11010", "100", "11010"]
test3 = [x++y | x<-["aab","dcc","aaa"], y<-["dcc", "aab"]]


