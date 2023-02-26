FetchContent_Declare(
	stbcontents
	GIT_REPOSITORY https://github.com/nothings/stb.git
)
FetchContent_MakeAvailable(stbcontents)

# Adding folder path in _stdSourceDir variable
FetchContent_GetProperties(stbcontents SOURCE_DIR _stdSourceDir)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${_stdSourceDir})