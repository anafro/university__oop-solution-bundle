if(EXISTS "D:/CLion Projects/bigint/cmake-build-debug/hello_test[1]_tests.cmake")
  include("D:/CLion Projects/bigint/cmake-build-debug/hello_test[1]_tests.cmake")
else()
  add_test(hello_test_NOT_BUILT hello_test_NOT_BUILT)
endif()