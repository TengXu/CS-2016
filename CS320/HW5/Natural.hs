module Natural
        where
import Unify

data Natural = Zero | Succ Natural | Var String
                 deriving (Show, Eq)

instance Substitutable Natural where
    subst (S a) Zero = Zero
    subst (S a) (Succ b) = Succ (subst (S a) b)
    subst (S a) (Var str)
               = case get str (S a) of
                Nothing ->  (Var str)
                Just value -> value

instance Unifiable Natural where
    unify Zero Zero = Just emp
    unify (Succ a) (Succ b) = unify a b
    unify (Var x) (Var y)
                    | x == y = Just emp
                    | otherwise = Just (S [(x,(Var y))])
    unify (Var x) b = Just (S [(x, b)])
    unify b (Var x) = Just (S [(x, b)])
    unify _ _ = Nothing
