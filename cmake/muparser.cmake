CPMAddPackage(
    NAME muparser
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdParty/muparser
    SYSTEM YES
    OPTIONS 
        "BUILD_SHARED_LIBS ON" 
        "ENABLE_SAMPLES OFF"
)