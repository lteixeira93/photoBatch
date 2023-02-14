set(FETCHCONTENT_QUIET OFF CACHE BOOL "Disable quiet mode in FetchContent" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG		   release-1.10.0
)
FetchContent_MakeAvailable(googletest)
