#!/usr/bin/env runghc

import           Control.Monad.Trans.Maybe
import           Text.Read                      ( readMaybe )
import           System.Environment             ( getArgs )
import           System.Exit                    ( exitFailure )
import           Control.Applicative            ( empty )
import           Control.Monad.State            ( StateT
                                                , get
                                                , put
                                                , runStateT
                                                )
import           Data.Char                      ( isDigit )

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

parseArgs :: IO [String] -> MaybeT IO Int
parseArgs = MaybeT . fmap (fmap fst . runStateT parseInput . unwords)

readFileInt :: String -> MaybeT IO Int
readFileInt = MaybeT . fmap readMaybe . readFile

type BrightCalc = StateT (Int, Int, Int) (MaybeT IO)

applyDiff :: BrightCalc Int
applyDiff = do
  (curr, full, diff) <- get
  return $ curr + diff * full `div` 100

restrict :: Int -> BrightCalc Int
restrict val = do
  (_, full, _) <- get
  if val < 0 then return 0 else if val > full then return full else return val

stale :: Int -> BrightCalc Int
stale val = do
  (curr, _, _) <- get
  if val == curr then empty else return val

newBrightness :: (Int, Int, Int) -> MaybeT IO Int
newBrightness = fmap fst . runStateT (applyDiff >>= restrict >>= stale)

main = do
  let currPath = "/sys/class/backlight/intel_backlight/brightness"
      fullPath = "/sys/class/backlight/intel_backlight/max_brightness"

  state <- runMaybeT $ do
    curr <- readFileInt currPath
    full <- readFileInt fullPath
    diff <- parseArgs getArgs
    newBrightness (curr, full, diff)

  case state of
    Nothing -> exitFailure
    Just a  -> writeFile currPath (show a)

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
