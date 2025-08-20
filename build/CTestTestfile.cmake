# CMake generated Testfile for 
# Source directory: /home/crem/chess_cpp
# Build directory: /home/crem/chess_cpp/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[chess.t]=] "/home/crem/chess_cpp/build/Debug/chess.t")
  set_tests_properties([=[chess.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/crem/chess_cpp/CMakeLists.txt;55;add_test;/home/crem/chess_cpp/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[chess.t]=] "/home/crem/chess_cpp/build/Release/chess.t")
  set_tests_properties([=[chess.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/crem/chess_cpp/CMakeLists.txt;55;add_test;/home/crem/chess_cpp/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[chess.t]=] "/home/crem/chess_cpp/build/RelWithDebInfo/chess.t")
  set_tests_properties([=[chess.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/crem/chess_cpp/CMakeLists.txt;55;add_test;/home/crem/chess_cpp/CMakeLists.txt;0;")
else()
  add_test([=[chess.t]=] NOT_AVAILABLE)
endif()
