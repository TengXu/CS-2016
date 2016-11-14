module Equation
(Equation(..),
solveEqn,
solveSystem)
        where
import Unify

data Equation a = a `Equals` a

solveEqn :: (Unifiable a) => Equation a -> Maybe (Subst a)
solveEqn (a `Equals` b) = unify a b

solveSystem :: (Unifiable a) => [Equation a] -> Maybe (Subst a)
solveSystem [] = Just (S [])
solveSystem (x:xs) = case solveEqn x of
                Nothing -> Nothing
                Just a -> cmb (Just a) (solveSystem xs)
