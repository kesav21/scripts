#!/usr/bin/env runghc

import           System.Environment             ( getArgs )
import           Text.Read                      ( readMaybe )

-- https://www.schoolofhaskell.com/user/bartosz/basics-of-haskell/3-pure-functions-laziness-io
-- https://www.schoolofhaskell.com/school/starting-with-haskell/basics-of-haskell/10_Error_Handling

toInt :: String -> Maybe Int
toInt = readMaybe

head' :: [a] -> Maybe a
head' []      = Nothing
head' (x : _) = Just x

dropLast :: [a] -> Maybe [a]
dropLast []       = Nothing
dropLast (x : []) = Just []
dropLast (x : xs) = (x :) <$> dropLast xs

midl' :: [a] -> Maybe [a]
midl' []       = Nothing
midl' (x : xs) = dropLast xs

last' :: [a] -> Maybe a
last' []       = Nothing
last' (x : []) = Just x
last' (x : xs) = last' xs

sign :: Char -> Maybe Int
sign '+' = Just 1
sign '-' = Just (-1)
sign _   = Nothing

isPercent :: Char -> Maybe Int
isPercent '%' = Just 1
isPercent _   = Nothing

parseArgs :: [String] -> Maybe Int
parseArgs args = do
  first     <- head' args
  sign'     <- head' first >>= sign
  magnitude <- midl' first >>= toInt
  percent   <- last' first >>= isPercent
  return $ sign' * magnitude * percent

restrict :: Int -> Int -> Int -> Int
restrict low high val | val < low  = low
                      | val > high = high
                      | otherwise  = val

stale :: Int -> Int -> Maybe Int
stale current new | current == new = Nothing
                  | otherwise      = Just new

calculate :: [String] -> String -> String -> Maybe Int
calculate args current full = do
  args'    <- parseArgs args
  current' <- toInt current
  full'    <- toInt full
  stale current' . restrict 0 full' $ current' + args' * full' `div` 100

main = do
  args    <- getArgs
  current <- readFile "/sys/class/backlight/intel_backlight/brightness"
  full    <- readFile "/sys/class/backlight/intel_backlight/max_brightness"

  case calculate args current full of
    Nothing -> print "error"
    Just x ->
      writeFile "/sys/class/backlight/intel_backlight/brightness" (show x)
