# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\CircularLogging_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CircularLogging_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\test_CircularLogger_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\test_CircularLogger_autogen.dir\\ParseCache.txt"
  "CircularLogging_autogen"
  "test_CircularLogger_autogen"
  )
endif()
