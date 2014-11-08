module Ffi where
import Foreign.Storable
import Foreign.Ptr
import Foreign.C

type Grid = Ptr (Ptr CInt)

foreign import ccall "ffi.h run"
    run :: Grid -> CChar -> Ptr CInt -> CInt -> IO CInt

foreign import ccall "ffi.h init"
    init :: Grid -> IO ()

foreign import ccall "ffi.h disp"
    disp :: Grid -> IO ()

{-foreign import ccall "ffi.h memGet"-}
    {-memGet :: Grid -> IO ()-}

foreign import ccall "ffi.h memFree"
    memFree :: Grid -> IO ()

foreign import ccall "ffi.h getScore"
    getScore :: IO CInt
    
foreign import ccall "ffi.h putScore"
    putScore :: CInt -> IO ()
--------------------------------------
foreign import ccall "run.h &GRID"
    pGrid :: Grid

foreign import ccall "run.h &SCORE"
    pScore :: Ptr CInt
    
foreign import ccall "run.h &bestScore"
    pBestScore :: Ptr CInt
---------------------------------------
listFromPtr :: Int -> Ptr CInt -> IO [Int]
listFromPtr n p = mapM toInt =<< mapM (peekElemOff p) [0 .. n-1] 

listFromPGrid :: Int -> Grid -> IO [[Int]]
listFromPGrid n pp = mapM (listFromPtr n) =<< mapM (peekElemOff pp) [0 .. n-1]

toInt :: CInt -> IO Int
toInt = return . fromEnum . fromIntegral
