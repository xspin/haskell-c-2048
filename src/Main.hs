-- -*- coding: utf-8 -*-
-- Copyright © 2014 Spin mail:code@e-spin.org

module Main where
import Graphics.UI.WX
import qualified Ffi
import Foreign.Storable
import Foreign.C

--consts
radius, maxX, maxY :: Int
maxX = 400
maxY = 400
radius = 44

type Grid = [Point]
type Tiles = [[Int]]

xgrid :: [Point]
xgrid = helper 0
  where 
  helper n
      | n < 16 = let t = 50 + n * 100 in let (y, x) = (50 + t `div` 400 * 100, t `mod` 400) in Point x y : helper (n+1)
      | otherwise = []

log2 :: Int -> Int
log2 n | n < 2 = 0
       | otherwise = 1 + log2 (n `div` 2)

colorText, cor :: [Color]
cor = (rgb 169 169 169) : [rgb 40 x x | x <- [20,40..]::[Int]]
colorText = [ rgb 255 255 255
            , rgb 216 191 216
            , rgb 255 105 180
            , rgb 128 0 128
            , rgb 138 43 226
            , rgb 70 130 180
            , rgb 72 209 204
            , rgb 60 179 113
            , rgb 173 255 47
            , rgb 128 128 0     -- Olive
            , rgb 210 105 30
            , rgb 255 0 0       -- Red
            , rgb 0 0 0         -- Black
            , rgb 255 255 0]

main :: IO ()
main = start gameFrame
-----------------------------------------------------------
gameFrame :: IO ()
gameFrame = do
    tiles       <- initGrid
    vTiles      <- varCreate tiles 
    vOldTiles   <- varCreate tiles
    vSt         <- varCreate 0
    cBest       <- Ffi.getScore         -- get the best score
    poke Ffi.pBestScore cBest

    f   <- frameFixed       [text := "2048-PUZZLE（×_×）"]
    p   <- panel f          []     
    tt  <- timer f          [interval := 100, on command := repaint p, enabled := False]
    t   <- timer f          [interval := 400]
    set p [on paint         := paintTiles tt vOldTiles vTiles vSt, bgcolor := grey]
    set t [on command       := repaintTiles '*' vTiles vSt p t, enabled := False]    
    set p [on (charKey 'w') := repaintTiles 'U' vTiles vSt p t
          ,on (charKey 'd') := repaintTiles 'R' vTiles vSt p t
          ,on (charKey 's') := repaintTiles 'D' vTiles vSt p t
          ,on (charKey 'a') := repaintTiles 'L' vTiles vSt p t
          ,on rightKey      := repaintTiles 'R' vTiles vSt p t
          ,on upKey         := repaintTiles 'U' vTiles vSt p t
          ,on downKey       := repaintTiles 'D' vTiles vSt p t
          ,on leftKey       := repaintTiles 'L' vTiles vSt p t
          ,on (charKey 'q') := close f
          ,on (charKey '-') := set t [interval :~ \i -> i*2]
          ,on (charKey '=') := set t [interval :~ \i -> max 1 (i `div` 2)]
          ,on (key KeySpace):= set t [enabled :~ not]     
          ,on click         := \_ -> set t [enabled :~ not]
          ,on clickRight    := \_ -> gameRestart vTiles vSt p
          ,on (charKey 'r') := gameRestart vTiles vSt p
          ]
    set f [layout := column 1 [ minsize (sz maxX (maxY+100)) $ margin 20 $ widget p
                              , marginLeft $ label "         2048 Puzzle (with AI) - Copyright©eSpin@2014"
                              ]]
    where
    initGrid :: IO Tiles
    initGrid = Ffi.init Ffi.pGrid >> Ffi.listFromPGrid 4 Ffi.pGrid

    gameRestart :: Var Tiles -> Var CInt -> Panel () -> IO()
    gameRestart vTiles vs p
      = do Ffi.memFree Ffi.pGrid
           ts <- initGrid
           varSet vTiles ts
           varSet vs 0
           poke Ffi.pScore 0
           repaint p

    paintTiles :: Timer -> Var Tiles -> Var Tiles -> Var CInt -> DC a -> Rect -> IO ()
    paintTiles t vOldTiles vTiles vSt dc viewArea
      = do
        old     <- varGet vOldTiles
        tiles   <- varGet vTiles
        score   <- peek Ffi.pScore
        s       <- varGet vSt
        best    <- peek Ffi.pBestScore
        let ct = concat tiles 
        let co = concat old
        let status | s == -1 = "Game Over! QAQ"
                   | s == 1 = "U Win! ^o^y"
                   | s == 2 = "Continue... 囧"
                   | otherwise = "→_→  (^_^)∠※ "
        if ct /= co then mapM_ (drawTile' dc) $ zip3 xgrid co $ zipWith (-) ct co
                    else mapM_ (drawTile dc) $ zip xgrid $ ct
        varSet vOldTiles tiles
        set t [enabled := (ct /= co)]
        drawRect dc (Rect 10 (maxY+10) 380 85) [brushColor := cyan, brushKind := BrushSolid]
        let instructions = init . unlines $
                              ["        -----Tips------" ++ "                             #SCORE:"
                              , "* Click/<Space> to use AI or not."
                              , "* Right-click/<R> to restart."
                              , "* <Q> to quit."
                              , "* <+/-> to change the speed." ]
        drawText dc instructions (Point 15 (maxY+15)) [color := black]
        drawText dc (show score) (Point 290 (maxY+30)) [color := red, fontSize := 12, fontWeight := WeightBold] -- show score
        drawText dc ("$BEST: " ++ show best) (Point 240 (maxY+50)) [color := blue, fontSize := 11]              -- show best score

        drawText dc status (Point 240 (maxY+70)) [color := yellow, fontSize := 10, fontWeight := WeightBold]    -- show status
        when (s == -1)              -- show GameOver
            $ drawText dc "Game Over! 〒_〒" (Point (40) (maxY `div` 2 - 20)) [color := red, fontSize := 25, fontWeight := WeightBold]
              >> drawText dc "Press R to restart!" (Point (90) (maxY `div` 2 + 30)) [color := yellow, fontSize := 17] 
        when (s == 1 && ct == co)   -- show Game win
            $ drawText dc "You Win!!  \\^o^/" (Point (80) (maxY `div` 2 - 20)) [color := red, fontSize := 20, fontWeight := WeightBold] 
              >> drawText dc "Continue? @_@a" (Point (110) (maxY - 120)) [color := blue, fontSize := 17] 
                >> varSet vSt 2
        when (score > best) $ poke Ffi.pBestScore score >> Ffi.putScore score                                   -- save the best score

        where
        drawTile dc (pt@(Point a b), num)
          = do circle dc pt radius [color := grey, brushColor := cor !! log2 num, brushKind := BrushSolid]
               when (num > 0) $ let d = ceiling $ logBase 10 (read $ show num) in
                                    drawText dc (show num) (Point (a-5*d) (b-10)) 
                                        [color := colorText !! (log2 num), fontWeight := WeightBold, fontSize := 14]
        drawTile' dc (pt@(Point a b), num, com)
          = do let delta | num == 0 = 0 | com > 0 = log2 com + 2 | com < 0 = - log2 com - 2 | otherwise = 0
               circle dc pt (radius + delta) [color := grey, brushColor := cor !! log2 num, brushKind := BrushSolid]
               when (num > 0) $ let d = ceiling $ logBase 10 (read $ show num) in
                                    drawText dc (show num) (Point (a-5*d) (b-10)) 
                                        [color := colorText !! (log2 num), fontWeight := WeightBold, fontSize := 14]
    
    repaintTiles :: Char -> Var Tiles -> Var CInt -> Panel () -> Timer -> IO ()
    repaintTiles key vTiles vs p t
      = do let ai = if key == '*' then 1 else 0
           let ch = castCharToCChar key
           status <- Ffi.run Ffi.pGrid ch Ffi.pScore ai
           s <- varGet vs
           when (status == 1 && s < 1) $ set t [enabled := False]
           when (s < 1) $ varSet vs status
           Ffi.listFromPGrid 4 Ffi.pGrid >>= varSet vTiles
           when (status == -1) $ set t [enabled := False] >> varSet vs status
           repaint p
