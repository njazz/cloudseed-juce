cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.11)
set(CMAKE_OSX_SYSROOT /)
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)

add_compile_options(-Wall)
# mostly for puredata sources:
#add_compile_options(-Wno-unused-parameter)

if(POLICY CMP0074)
    #policy for <PackageName>_ROOT variables
    cmake_policy(SET CMP0074 NEW)
endif()


set (SRC

src/AllpassDiffuser.h
src/Default.h
src/DelayLine.h
src/Exports.cpp
src/FastSin.cpp
src/FastSin.h
src/ModulatedAllpass.h
src/ModulatedDelay.h
src/MultitapDiffuser.h
src/Parameter.h
src/ReverbChannel.h
src/ReverbController.h
src/Utils.h

src/AudioLib/Biquad.cpp
src/AudioLib/Biquad.h
src/AudioLib/Hp1.h
src/AudioLib/Lp1.h
src/AudioLib/MathDefs.h
src/AudioLib/ShaRandom.cpp
src/AudioLib/ShaRandom.h
src/AudioLib/ValueTables.cpp
src/AudioLib/ValueTables.h

src/Utils/Sha256.cpp
src/Utils/Sha256.h

)

add_library (CloudSeedNative STATIC)
target_sources(CloudSeedNative PRIVATE ${SRC})
target_include_directories(CloudSeedNative PRIVATE ${CMAKE_SOURCE_DIR}/src/)
