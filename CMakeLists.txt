cmake_minimum_required(VERSION 3.12)

project(PhotoBatch)

add_executable(PhotoBatch 
	"PhotoBatch.cpp" 
	"ArgumentParser.h"
	"ArgumentParser.cpp"
	"Utils.h"
	"Utils.cpp"
	"Unittests/TestArgumentParser.cpp"
)

set_target_properties(PhotoBatch PROPERTIES CXX_STANDARD 17)
add_subdirectory(Unittests)