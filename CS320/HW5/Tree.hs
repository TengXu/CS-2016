module Tree
    where
import Unify
import Equation

data Tree = Leaf | Node Tree Tree | Var String
                deriving (Show, Eq)

instance Substitutable Tree where
    subst (S a) Leaf = Leaf
    subst (S a) (Node x y) = Node (subst (S a) x) (subst (S a) y)
    subst (S a) (Var str)
               = case get str (S a) of
                Nothing ->  (Var str)
                Just value -> value

instance Unifiable Tree where
    unify Leaf Leaf = Just emp
    unify (Node al ar) (Node bl br) = cmb (unify al bl) (unify ar br)
    unify (Var x) (Var y)
                    | x == y = Just emp
                    | otherwise = Just (S [(x,(Var y))])
    unify (Var x) b = Just (S [(x, b)])
    unify b (Var x) = Just (S [(x, b)])
    unify _ _ = Nothing

e0 = Node (Node (Node (Var "x") (Var "y")) (Node (Var "y") (Var "x"))) (Var "z") `Equals` Node (Node (Node Leaf (Var "z")) (Node Leaf (Var "y"))) (Var "x")
e1 = let f b 0 = b
         f b n = Node (f b (n-1)) (f b (n-1))
     in f (Var "x") 10 `Equals` f Leaf 13
e2 = [ (Var "z") `Equals` Leaf, Node (Var "y") Leaf `Equals` Node Leaf (Var "x"), (Var "x") `Equals` Node (Var "z") (Var "z")]

s0 = Just (S [("y",Var "z"),("z",Leaf),("x",Leaf)])
s1 = Just (S [("x",Node (Node (Node Leaf Leaf) (Node Leaf Leaf)) (Node (Node Leaf Leaf) (Node Leaf Leaf)))])
s2 = Nothing
