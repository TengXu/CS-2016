----------------------------------------------------------------
-- Computer Science 320 (Fall, 2016)
-- Concepts of Programming Languages
--
-- Assignment 6
--   Parser.hs

----------------------------------------------------------------
-- Parser for the mini-Haskell Interpreter

module Parser (parseFile) where

import Text.ParserCombinators.Parsec
import qualified Text.ParserCombinators.Parsec.Token as P
import Text.ParserCombinators.Parsec.Expr
import Text.ParserCombinators.Parsec.Language

import Exp

----------------------------------------------------------------
-- Exported Functions

parseFile :: SourceName -> IO (Either ParseError Exp)
parseFile = parseFromFile program

----------------------------------------------------------------
-- Top-level Parser

program :: Parser Exp
program
    = do{ whiteSpace
        ; e <- exprParser
        ; eof
        ; return e
        }

----------------------------------------------------------------
-- Expression Parser

exprParser :: Parser Exp
exprParser
    = choice 
      [ appParser
      , exprNoAppParser
      ]
    <?> "expression"

exprNoAppParser
    = choice 
      [ varParser
      , intParser
      , boolParser
      , wopOrUnitOrAtomParser
      , opParserN
      , nilParser
      , lamParser
      , ifParser
      , letParser
      ]
    <?> "expression"

letParser
    = do{ reserved "let"
        ; x <- identifier
        ; reserved "="
        ; e <- exprParser
        ; rest <- choice [bindParser [(x,e)], 
                          inParser [(x,e)]]
        ; return rest
        }
    <?> "let binding"

bindParser binds
    = do{ reserved ";"
        ; x <- identifier
        ; reserved "="
        ; e <- exprParser
        ; rest <- choice [bindParser ((x, e):binds),
                          inParser ((x,e):binds)]
        ; return rest
        }
    <?> "let binding"

inParser binds
    = do{ reserved "in"
        ; e <- exprParser
        ; return (Let (reverse binds) e)
        }
    <?> "let body"

foldApp f []     = f
foldApp f (e:[]) = App f e
foldApp f (e:es) = foldApp (App f e) es

appParser
    = do{ es <- many1 exprNoAppParser
        ; return (foldApp (head es) (tail es))
        }
    <?> "application"

lamParser
    = do{ symbol "\\"
        ; x <- choice [idOrUnit1Parser, idOrUnit2Parser]
        ; symbol "->"
        ; e <- exprParser
        ; return $ (case x of Right x -> Lam x e
                              Left () -> LamUnit e)
        }
    <?> "lambda expression"

idOrUnit1Parser
    = do{ x <- identifier
        ; return $ Right x
        }
    <?> "variable name"
    
idOrUnit2Parser
    = do{ symbol "()"
        ; return $ Left ()
        }
    <?> "unit"

ifParser
    = do{ reserved "if"
        ; e1 <- wopOrUnitOrAtomParser
        ; reserved "then"
        ; e2 <- exprParser
        ; reserved "else"
        ; e3 <- exprParser
        ; return $ If e1 e2 e3
        }
    <?> "if expression"

wopOrUnitOrAtomParser
    = do{ symbol "("
        ; r <- choice [opParserW, unitParser, atomExprParser]
        ; return r
        }
    <?> "wrapped expression, operator, or unit"

atomExprParser
    = do{ e <- exprParser
        ; symbol ")"
        ; return e
        }
    <?> "wrapped expression"

unitParser
    = do{ symbol ")"
        ; return Unit
        }
    <?> "unit"

nilParser
    = do{ symbol "[]"
        ; return Nil
        }
    <?> "nil"

opParserW
    = do{ op <- choice (map (uncurry opParser) 
                            opsWithStrings2)
        ; return (Op op)
        }
    <?> "wrapped operator"

opParserN
    = do{ op <- choice (map (uncurry opParser)
                       (take 3 opsWithStrings))
        ; return (Op op)
        }
    <?> "operator"

opParser or os
    = do{ symbol os
        ; return or
        }
    <?> "operator"

boolParser
    = choice 
      [ trueParser
      , falseParser
      ]
    <?> "boolean constant"

trueParser
    = do{ symbol "True"
        ; return (B True)
        }
    <?> "boolean constant: True"

falseParser
    = do{ symbol "False"
        ; return (B False)
        }
    <?> "boolean constant: False"

varParser
    = do{ i <- identifier
        ; return (Var i)
        }
    <?> "variable name"

intParser
    =   choice 
      [ numParser,
        numNegParser
      ]
    <?> "integer"

numParser
    =   do{ n <- natural
          ; return (N $ fromInteger n)
          }
    <?> "positive integer"

numNegParser
    =   do{ symbol "-"
          ; n <- natural
          ; return (N (-1 * (fromInteger n)))
          }
    <?> "negative integer"

----------------------------------------------------------------
-- Parsec Definitions

langDef
    = haskellStyle
    { identStart        = letter
    , identLetter       = alphaNum <|> oneOf "_'"
    , opStart           = opLetter langDef
    , opLetter          = oneOf "(=\\[;"
    , reservedOpNames   = ["=","(+)","(*)","(==)",
                           "(&&)","(||)","(:)",
                           "\\","[]","()"
                           , ";"]  
    , reservedNames     = [ "True", "False",
                            "not", "head", "tail",
                            "if", "then", "else",
                            "let", "in"]
    }

lang            = P.makeTokenParser langDef

whiteSpace      = P.whiteSpace lang
symbol          = P.symbol lang
identifier      = P.identifier lang
reserved        = P.reserved lang
natural         = P.natural lang
