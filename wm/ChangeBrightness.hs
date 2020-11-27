#!/usr/bin/env runghc

import           System.Environment             ( getArgs )
import           System.Exit                    ( exitFailure )
import           Text.Read                      ( readMaybe )

-- https://www.schoolofhaskell.com/user/bartosz/basics-of-haskell/3-pure-functions-laziness-io
-- https://www.schoolofhaskell.com/school/starting-with-haskell/basics-of-haskell/10_Error_Handling
-- https://mmhaskell.com/monads/transformers

toEither :: String -> Maybe a -> Either String a
toEither msg Nothing      = Left msg
toEither msg (Just first) = Right first

argError = toEither "Provide an argument of the form \\+\\d+%"
signError = toEither "Provide an argument of the form \\+\\d+%"
magError = toEither "Provide an argument of the form \\+\\d+%"
percentError = toEither "Provide an argument of the form \\+\\d+%"
readCurrentError = toEither "Failed to read current brightness"
readFullError = toEither "Failed to read max brightness "
staleError = toEither "No change in brightness"

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

parseArgs :: [String] -> Either String Int
parseArgs args = do
  first     <- argError (head' args)
  sign'     <- signError (head' first >>= sign)
  magnitude <- magError (midl' first >>= toInt)
  percent   <- percentError (last' first >>= isPercent)
  return $ sign' * magnitude * percent

restrict :: Int -> Int -> Int -> Int
restrict low high val | val < low  = low
                      | val > high = high
                      | otherwise  = val

stale :: Int -> Int -> Maybe Int
stale current new | current == new = Nothing
                  | otherwise      = Just new

calculate :: [String] -> String -> String -> Either String Int
calculate args curr full = do
  args' <- parseArgs args
  curr' <- readCurrentError (toInt curr)
  full' <- readFullError (toInt full)
  staleError . stale curr' . restrict 0 full' $ curr' + args' * full' `div` 100

main = do
  let currPath = "/sys/class/backlight/intel_backlight/brightness"
      fullPath = "/sys/class/backlight/intel_backlight/max_brightness"

  args <- getArgs
  curr <- readFile currPath
  full <- readFile fullPath

  case calculate args curr full of
    Left e -> do
      putStrLn e
      exitFailure
    Right x -> do
      print x
      writeFile currPath (show x)
