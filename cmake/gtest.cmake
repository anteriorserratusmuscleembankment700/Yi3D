set(GOOGLETEST_VERSION "1.14.0")
CPMAddPackage(
    NAME gtest
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdParty/googletest/googletest
    SYSTEM YES
    OPTIONS "gtest_force_shared_crt ON" "BUILD_GMOCK OFF"
)
