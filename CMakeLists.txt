cmake_minimum_required(VERSION 3.25)

include (${CMAKE_CURRENT_LIST_DIR}/cmake/quest.cmake)

project(kaleb-test)

include(${CMAKE_CURRENT_LIST_DIR}/shared/cmake/assets.cmake)

add_assets(kaleb-assets SHARED "${CMAKE_CURRENT_LIST_DIR}/assets" "${CMAKE_CURRENT_LIST_DIR}/include/assets.hpp")
