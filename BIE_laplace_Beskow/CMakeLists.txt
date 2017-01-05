cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(BIE_laplace C)

enable_language(C)
enable_language(Fortran)


set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}${GCC_COVERAGE_LINK_FLAGS}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
#set(OWN_GSL /cfs/klemming/nobackup/f/ffry/hpc_summerschool2016/gsl)
set(OWN_GSL /cfs/klemming/nobackup/s/sarapal/project/BIE_Laplace_PDC/gsl)
set(GSL_DIR "OWN_GSL")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/api)

find_package(vecLib REQUIRED)
#find_package(GSL REQUIRED)

include_directories(vecLib_INCLUDE_DIR)
include_directories(${PROJECT_SOURCE_DIR}/api)
include_directories(${OWN_GSL}/include/)

link_directories(${OWN_GSL}/lib/)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")

add_executable(
main_exe
src/main.c
src/init.c
src/initFunction.c
src/solveDensity.c
src/computeSolution.c
src/specialquadlapl.c
src/computeError.c
)

#set(GSL_LIBRARIES "${OWN_GSL}/lib/")
target_link_libraries(main_exe ${vecLib_LINKER_LIBS})
target_link_libraries(main_exe gsl gslcblas)
target_link_libraries(main_exe  m)
