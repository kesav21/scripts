#!/usr/bin/env runghc

import           Control.Monad.Reader
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

readFileInt :: String -> MaybeT IO Int
readFileInt = MaybeT . fmap readMaybe . readFile

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

type BrightCalc = ReaderT (Int, Int, Int) (MaybeT IO) Int

applyDiff :: BrightCalc
applyDiff = do
  (curr, full, diff) <- ask
  return $ curr + diff * full `div` 100

restrict :: Int -> BrightCalc
restrict val = do
  (_, full, _) <- ask
  if val < 0 then return 0 else if val > full then return full else return val

stale :: Int -> BrightCalc
stale val = do
  (curr, _, _) <- ask
  if val == curr then empty else return val

newBrightness :: (Int, Int, Int) -> MaybeT IO Int
newBrightness = runReaderT $ applyDiff >>= restrict >>= stale

-- newBrightness :: (Int, Int, Int) -> MaybeT IO Int
-- newBrightness (curr, full, diff) = MaybeT $ do
--   return . stale curr . restrict 0 full . calculate $ (curr, full, diff)
--  where
--   stale old new | old == new = Nothing
--                 | otherwise  = Just new
--   restrict low high val | val < low  = low
--                         | val > high = high
--                         | otherwise  = val
--   calculate (curr, full, diff) = curr + diff * full `div` 100

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
