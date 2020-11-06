#!/usr/bin/env runghc

import           System.Environment             ( getArgs )
import           Control.Applicative            ( Alternative
                                                , empty
                                                , (<|>)
                                                )
import           Data.Char                      ( isDigit )
import           Data.Maybe                     ( catMaybes )

newtype Parser a = Parser { runParser:: String -> Maybe (a, String) }

instance Functor Parser where
  fmap f (Parser p) = Parser $ \input -> do
    (parsed, input') <- p input
    return (f parsed, input')

instance Applicative Parser where
  pure a = Parser $ \input -> Just (a, input)
  (Parser p1) <*> (Parser p2) = Parser $ \input -> do
    (f, input' ) <- p1 input
    (a, input'') <- p2 input'
    return (f a, input'')

instance Alternative Parser where
  empty = Parser $ \_ -> Nothing
  (Parser p1) <|> (Parser p2) = Parser $ (<|>) <$> p1 <*> p2

instance Monad Parser where
  return a = Parser $ \input -> Just (a, input)
  (Parser p1) >>= f = Parser $ \input -> do
    (a, input') <- p1 input
    runParser (f a) input'

notNull :: Parser [a] -> Parser [a]
notNull (Parser p) = Parser $ \input -> do
  (a, input') <- p input
  if null a then Nothing else Just (a, input')

parseChar :: Char -> Parser Char
parseChar char = Parser parser
 where
  parser (x : xs) | x == char = Just (x, xs)
  parser _                    = Nothing

parseInt :: Parser Int
parseInt = fmap read . notNull . Parser $ Just . span isDigit

parseInput :: Parser Int
parseInput = do
  sign      <- parseChar '+' <|> parseChar '-'
  magnitude <- parseInt
  parseChar '%'
  case sign of
    '+' -> return magnitude
    '-' -> return (-magnitude)
    _   -> empty

parseArgs :: [String] -> [Int]
parseArgs = map fst . catMaybes . map (runParser parseInput)

main = getArgs >>= print . parseArgs
-- main = getArgs >>= print

-- main = do
--   print $ runParser (parseChar '+') ""
--   print $ runParser (parseChar '+') "5%"
--   print $ runParser (parseChar '+') "+"
--   print $ runParser (parseChar '+') "+5%"
--   putStrLn ""
--   print . runParser parseInt $ ""
--   print . runParser parseInt $ "5%"
--   print . runParser parseInt $ "10%"
--   putStrLn ""
--   print . runParser parseInput $ ""
--   print . runParser parseInput $ "-"
--   print . runParser parseInput $ "+"
--   print . runParser parseInput $ "5"
--   print . runParser parseInput $ "%"
--   print . runParser parseInput $ "+5"
--   print . runParser parseInput $ "5%"
--   print . runParser parseInput $ "+5%"
--   print . runParser parseInput $ "-5%"
--   putStrLn ""
--   args <- getArgs
--   print . parseArgs $ args


-- Parser.hs sign magnitude

-- sign
-- -i, --increase
-- -d, --decrease

-- magnitude
-- -p n, --percentage n
-- number, 1-100, 0 < new < max
-- -a n, --absolute n
-- number, 1-max, 0 < new < max

