add_library(
  tastyhedge_lib

  "Analytics/Model_BlackScholes.cpp"
  
  "Core/Csv.cpp"
  "Core/FileIO.cpp"
  "Core/Logger.cpp"
  "Core/Time.cpp"
  
  "Data/Navigator.cpp"
  "Data/RatesHub.cpp"
  "Data/Tape.cpp"

  "_public/lets_be_rational/erf_cody.cpp"
  "_public/lets_be_rational/lets_be_rational.cpp"
  "_public/lets_be_rational/normaldistribution.cpp"
  "_public/lets_be_rational/rationalcubic.cpp"
)


# Link Docopt
find_package(
    docopt CONFIG REQUIRED)

# Link QuantLib
find_package(
    QuantLib CONFIG REQUIRED)


target_include_directories(
    tastyhedge_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    tastyhedge_lib
	PRIVATE
        QuantLib::QuantLib
)


### tastyhedge
###
add_executable(
  tastyhedge

  "tastyhedge.cpp"
  "version.cpp"
)

### tasty::Version
###
### From https://www.mattkeeter.com/blog/2018-01-06-versioning/
###
include(version.cmake)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/_version.cpp
    COMMAND
        ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/version.cmake
)

target_link_libraries(
    tastyhedge
	PRIVATE
        tastyhedge_lib

        $<IF:$<TARGET_EXISTS:docopt>,docopt,docopt_s>
)
