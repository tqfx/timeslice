# include modules
include(${CMAKE_CURRENT_LIST_DIR}/core.cmake)

macro(sanitize_append)
  list_append(C_FLAGS ${ARGN})
  list_append(CXX_FLAGS ${ARGN})
endmacro()

if("${CMAKE_C_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR "${CMAKE_C_COMPILER_ID}" MATCHES "IntelLLVM")
  sanitize_append(-fsanitize=address -fno-omit-frame-pointer -fsanitize-recover=address -fsanitize=undefined)
  if(NOT ("${CMAKE_C_COMPILER_ID}" MATCHES "Apple[Cc]lang"))
    sanitize_append(-fsanitize=leak)
  endif()
elseif("${CMAKE_C_COMPILER_ID}" MATCHES "MSVC")
  sanitize_append(/fsanitize=address)
endif()
