module Unify
(get,
emp,
sub,
Substitutable(..),
Unifiable(..),
Subst(..),
unify,
subst,
unresolved,
resolve,
cmb)
    where

data Subst a = S [(String, a)]
            deriving(Show)

emp :: Subst a
emp = S []

sub :: String -> a -> Subst a
sub str a = S [(str,a)]

get :: String -> Subst a -> Maybe a
get str (S []) = Nothing
get str (S (x:xs))
            | (fst x) == str =  Just (snd x)
            | otherwise = get str (S xs)

class Substitutable a where
    subst::Subst a -> a -> a

class (Eq a, Substitutable a) => Unifiable a where
    unify::a -> a -> Maybe (Subst a)

unresolved :: [(String,a)] -> Maybe (String, a, a)
unresolved [] = Nothing
unresolved (x:xs)
    | isMember (fst x) xs == False = unresolved xs
    | otherwise = getMember x xs

--helper function
isMember :: String -> [(String,a)] -> Bool
isMember _ [] = False
isMember str xs
    | str == (fst (head xs)) = True
    | otherwise = isMember str (tail xs)

getMember :: (String,a) -> [(String,a)] -> Maybe (String, a,a)
getMember _ [] = Nothing
getMember x (y:ys)
    | (fst x) == (fst y) = Just (fst x, snd x, snd y)
    | otherwise = getMember x ys

resolve::Unifiable a => Subst a -> Maybe (Subst a)
resolve (S a)
        = case unresolved a of
         Nothing ->  Just (S a)
         Just (x, y, z) -> case unify y z of
                            Nothing -> Nothing
                            Just (S o) -> resolve (S ((removeMember a (Just (x,y,z))) ++ o))

removeMember x (Just (a,b,c)) = [y| y <-x, (fst y) /= a] ++ [(a, b)]

cmb :: Unifiable a => Maybe (Subst a) -> Maybe (Subst a) -> Maybe (Subst a)
cmb Nothing _ = Nothing
cmb _ Nothing = Nothing
cmb (Just (S a)) (Just (S b)) = resolve (S (a++b))


