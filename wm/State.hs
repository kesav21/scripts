#!/usr/bin/env runghc

import           Control.Monad.State

pop :: State String Char
pop = state parser
 where
  parser (x : xs) = (x, xs)
  parser xs       = ('k', xs)

push :: Char -> State String ()
push a = state $ \xs -> ((), a : xs)

-- get :: State String String
-- get = state $ \s -> (s, s)

-- put :: String -> State String ()
-- put newState = state $ \s -> ((), newState)

isEq :: Eq a => a -> a -> Maybe a
isEq a b | a == b    = Just a
         | otherwise = Nothing

parseChar :: Char -> State String (Maybe Char)
parseChar char = pop >>= return . isEq char

parseSign :: State String Int
parseSign = do
  a <- pop
  case a of
    '+' -> return 1
    '-' -> return (-1)
    _   -> return 0


main = do
  print . runState (parseChar '+') $ ""
  print . runState (parseChar '+') $ "-"
  print . runState (parseChar '+') $ "+"
  -- print . runState (parseChar '+' <|> parseChar '-') $ ""
  -- putStrLn ""
  -- print . runState parseSign $ ""
  -- print . runState parseSign $ "-"
  -- print . runState parseSign $ "+"
  -- putStrLn ""
  -- print . runState parseInt $ ""
  -- print . runState parseInt $ "-"
  -- print . runState parseInt $ "+"
