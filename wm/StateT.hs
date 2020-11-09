#!/usr/bin/env runghc

import           System.Environment             ( getArgs )
import           Control.Applicative            ( empty
                                                , (<|>)
                                                )
import           Control.Monad.State            ( StateT
                                                , get
                                                , put
                                                , runStateT
                                                )
import           Data.Char                      ( isDigit )
import           Data.Maybe                     ( catMaybes )

type Parser = StateT String Maybe

parseChar :: Char -> Parser Char
parseChar char = do
  (x : xs) <- get
  put xs
  if x /= char then empty else return x

parseInt :: Parser Int
parseInt = do
  (x, xs) <- span isDigit <$> get
  put xs
  if null x then empty else return $ read x

parseInput :: Parser Int
parseInput = do
  sign      <- parseChar '+' <|> parseChar '-'
  magnitude <- parseInt
  _         <- parseChar '%'
  case sign of
    '+' -> return magnitude
    '-' -> return (-magnitude)
    _   -> empty

parseArgs :: [String] -> [Int]
parseArgs = map fst . catMaybes . map (runStateT parseInput)

main = getArgs >>= print . parseArgs

-- main = do
--   print $ runStateT (parseChar '+') ""
--   print $ runStateT (parseChar '+') "5%"
--   print $ runStateT (parseChar '+') "+"
--   print $ runStateT (parseChar '+') "+5%"
--   putStrLn ""
--   print $ runStateT parseInt ""
--   print $ runStateT parseInt "5%"
--   print $ runStateT parseInt "10%"
--   putStrLn ""
--   print $ runStateT parseInput ""
--   print $ runStateT parseInput "-"
--   print $ runStateT parseInput "+"
--   print $ runStateT parseInput "5"
--   print $ runStateT parseInput "%"
--   print $ runStateT parseInput "+5"
--   print $ runStateT parseInput "5%"
--   print $ runStateT parseInput "+5%"
--   print $ runStateT parseInput "-5%"
