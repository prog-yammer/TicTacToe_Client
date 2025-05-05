# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Client_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Client_autogen.dir/ParseCache.txt"
  "CMakeFiles/test_auth_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/test_auth_autogen.dir/ParseCache.txt"
  "CMakeFiles/test_game_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/test_game_autogen.dir/ParseCache.txt"
  "CMakeFiles/test_lobby_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/test_lobby_autogen.dir/ParseCache.txt"
  "Client_autogen"
  "test_auth_autogen"
  "test_game_autogen"
  "test_lobby_autogen"
  )
endif()
