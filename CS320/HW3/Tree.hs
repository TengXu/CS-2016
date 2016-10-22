-- Teng Xu xt@bu.edu
-- HW3

--Leaf :: Tree a
--Node :: a -> Tree a -> Tree a -> Tree a
data Tree a = Leaf
            | Node a (Tree a) (Tree a)
              deriving (Show, Eq)

--mapT :: (a -> b) -> Tree a -> Tree b
mapT f Leaf = Leaf
mapT f (Node a left right) = Node (f a) (mapT f left) (mapT f right)

--foldT :: (a -> b -> b -> b) -> b -> Tree a -a> b
foldT f b Leaf = b
foldT f b (Node a left right) = f a (foldT f b left) (foldT f b right)

--leafCount :: Tree a -> Integer
leafCount Leaf = 1
leafCount (Node a left right) = foldT sum 1 (Node a left right)
                                    where
                                    sum x y z = y + z

--nodeCount :: Tree a -> Integer
nodeCount Leaf = 0
nodeCount (Node a left right) = foldT sum2 0 (Node a left right)
                                    where
                                    sum2 x y z = 1 + y + z

--height :: Tree a -> Integer
height Leaf = 0
height (Node a left right) = foldT sum3 0 (Node a left right)
                                    where
                                    sum3 x y z
                                        | y > z = 1 + y
                                        | otherwise = 1 + z

--perfect :: Tree a -> Bool
perfect Leaf = True
perfect (Node a left right)
    | ((height left) == (height right)) = (perfect left) && (perfect right)
    | otherwise = False

--degenerate :: Tree a -> Bool
degenerate Leaf = True
degenerate (Node a left right)
    | (left == Leaf || right == Leaf) = (degenerate left) && (degenerate right)
    | otherwise = False

--list :: Tree a -> Maybe [a]
list Leaf = []
list (Node a left right)
    | (degenerate (Node a left right) == False) = []
    | otherwise = foldT f [] (Node a left right)
                  where
                  f x y z = [x] ++ y ++ z
