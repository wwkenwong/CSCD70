set(TENSORFLOW_DEV_DIR ${CMAKE_CURRENT_LIST_DIR}/../tensorflow-dev)

if (NOT EXISTS ${TENSORFLOW_DEV_DIR})
    message(FATAL_ERROR "Directory ${TENSORFLOW_DEV_DIR} does not exist. "
                        "Please make sure to run installTensorFlow.sh.")
endif()

file(GLOB PYTHON_INCLUDE_DIR ${TENSORFLOW_DEV_DIR}/include/python*)
file(GLOB PYTHON_LIB_DIR     ${TENSORFLOW_DEV_DIR}/lib/python*)

if (NOT TENSORFLOW_MAJOR_VERSION)
    set(TENSORFLOW_MAJOR_VERSION 2)
endif()

find_path(TENSORFLOW_CORE_DIR
          NAMES libtensorflow_framework.so.${TENSORFLOW_MAJOR_VERSION}
          PATHS ${PYTHON_LIB_DIR}/site-packages/tensorflow
                ${PYTHON_LIB_DIR}/site-packages/tensorflow_core)

get_filename_component(PYTHON_LIB ${PYTHON_INCLUDE_DIR} NAME)

find_path(PYTHON_INCLUDE_DIR
          NAMES "Python.h"
          PATHS ${PYTHON_INCLUDE_DIR})
find_path(TENSORFLOW_INCLUDE_DIR
          NAMES "tensorflow"
          PATHS ${TENSORFLOW_CORE_DIR} PATH_SUFFIXES include)
find_path(TENSORFLOW_PYWRAP_INTERNAL_PATH
          NAMES "_pywrap_tensorflow_internal.so"
          PATHS ${TENSORFLOW_CORE_DIR}/python)

find_library(TENSORFLOW_FRAMEWORK_LIB
             libtensorflow_framework.so.${TENSORFLOW_MAJOR_VERSION}
             HINTS ${TENSORFLOW_CORE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TensorFlow DEFAULT_MSG
    TENSORFLOW_CORE_DIR
    PYTHON_INCLUDE_DIR
    TENSORFLOW_INCLUDE_DIR
    TENSORFLOW_FRAMEWORK_LIB
    TENSORFLOW_PYWRAP_INTERNAL_PATH)

if (TensorFlow_FOUND)
    message(STATUS "Found TensorFlow (include: ${TENSORFLOW_INCLUDE_DIR}, "
                   "lib: ${TENSORFLOW_FRAMEWORK_LIB} "
                   "${TENSORFLOW_PYWRAP_INTERNAL_PATH})")
endif()
