#
# Copyright (c) 2017 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

# native code directory
set(BASE_PATH_FOR_THIS_MODULE "${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}/Windows.Devices.SerialCommunication")


# set include directories
list(APPEND Windows.Devices.SerialCommunication_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/CLR/Core")
list(APPEND Windows.Devices.SerialCommunication_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/CLR/Include")
list(APPEND Windows.Devices.SerialCommunication_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/HAL/Include")
list(APPEND Windows.Devices.SerialCommunication_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/PAL/Include")
list(APPEND Windows.Devices.SerialCommunication_INCLUDE_DIRS "${BASE_PATH_FOR_THIS_MODULE}")


# source files
set(Windows.Devices.SerialCommunication_SRCS

    win_dev_serial_native_Windows_Devices_SerialCommunication_SerialDevice.cpp
    win_dev_serial_native.cpp
)

foreach(SRC_FILE ${Windows.Devices.SerialCommunication_SRCS})
    set(Windows.Devices.SerialCommunication_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(Windows.Devices.SerialCommunication_SRC_FILE ${SRC_FILE}
        PATHS 
            "${BASE_PATH_FOR_THIS_MODULE}"

        CMAKE_FIND_ROOT_PATH_BOTH
    )
    # message("${SRC_FILE} >> ${Windows.Devices.SerialCommunication_SRC_FILE}") # debug helper
    list(APPEND Windows.Devices.SerialCommunication_SOURCES ${Windows.Devices.SerialCommunication_SRC_FILE})
endforeach()


include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(NF_Debugger DEFAULT_MSG Windows.Devices.SerialCommunication_INCLUDE_DIRS Windows.Devices.SerialCommunication_SOURCES)