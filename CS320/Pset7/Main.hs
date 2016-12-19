----------------------------------------------------------------
-- Computer Science 320 (Fall, 2016)
-- Concepts of Programming Languages
--
-- Assignment 6
--   Main.hs

----------------------------------------------------------------
-- Main Module for the mini-Haskell Interpreter

module Main where

--import System.Environment   -- to obtain command-line arguments

import Err (Error(..))
import Eval (evalExp)
import Parser (parseFile)

----------------------------------------------------------------
-- mainParseEval takes a file path in the form of a string,
-- tries to parse the file into an abstract syntax,
-- and if it succeeds, it evaluates the abstract syntax,
-- displaying the result (or error) returned by the
-- evaluation function. 

mainParseEval :: String -> IO ()
mainParseEval fname =
  do { r <- parseFile fname
     ; case r of              
       Left err ->
         do { putStr "parse error: "
            ; print err
            }
       Right e ->
         case (evalExp e) of
            Error msg ->
              do { putStr "evaluation error: "
                 ; print msg
                 }
            S e' -> print e'
     }

----------------------------------------------------------------
-- the main function, useful if the interpreter is compiled.

main :: IO ()
main = mainParseEval "tests2.mhs"

-- If you wish to compile the interpreter, the following
-- code can be used to obtain a file name from the command
-- line. However, be sure to uncomment the
--
--      import System.Environment
--
-- at the beginning of this module's definition.

-- main takes the first command-line argument and treats it
-- as a file path, calling mainParseRun on that file; if
-- the number of command-line argument is not exactly one,
-- main returns an error message indicating this.

-- main =
--    do { args <- getArgs
--       ; case args of
--          [filename] -> 
--            mainParseEval (head args)
--          otherwise  -> 
--            putStr "error: specify a single file name.\n" }
