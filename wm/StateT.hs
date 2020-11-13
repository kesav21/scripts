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

parseWord :: Parser String
parseWord = do
  (x, xs) <- span (/= ' ') <$> get
  put xs
  if null x then empty else return x

parseInt :: Parser Int
parseInt = do
  (x, xs) <- span isDigit <$> get
  put xs
  if null x then empty else return $ read x

parseSign :: Parser Int
parseSign = do
  word <- parseWord
  _    <- parseChar ' '
  case word of
    "-i"         -> return 1
    "--increase" -> return 1
    "-d"         -> return (-1)
    "--decrease" -> return (-1)
    _            -> empty

parseMagnitude :: Parser Int
parseMagnitude = do
  word <- parseWord
  _    <- parseChar ' '
  case word of
    "-p"           -> parseInt >>= return
    "--percentage" -> parseInt >>= return
    _              -> empty

parseInput :: Parser Int
parseInput = do
  sign      <- parseSign
  magnitude <- parseMagnitude
  return $ sign * magnitude

parseArgs :: [String] -> Maybe Int
parseArgs = fmap fst . runStateT parseInput . unwords

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
