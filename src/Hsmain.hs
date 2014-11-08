module Hsmain where
import qualified Ffi
import Foreign.Storable
import Foreign.Ptr
import Foreign.C

main = do
    print "Hey, 2048 start!"
    {-Ffi.memGet Ffi.pGrid-}
    Ffi.init Ffi.pGrid
    print "Init Sccuced!"
    Ffi.disp Ffi.pGrid
    let ch = castCharToCChar 'U'
    Ffi.run Ffi.pGrid ch Ffi.pScore 1
    Ffi.disp Ffi.pGrid
    Ffi.listFromPGrid 4 Ffi.pGrid >>= print
    {-Ffi.memFree Ffi.pGrid-}
    print "Game End!"
