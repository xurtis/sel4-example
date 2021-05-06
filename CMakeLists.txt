#
# Copyright 2021, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.7.2)

include(settings.cmake)

project(example C CXX ASM)

find_package(seL4 REQUIRED)
find_package(elfloader-tool REQUIRED)
find_package(sel4runtime REQUIRED)
find_package(util_libs REQUIRED)

# Set the roottask CNode size to be large enough for DTB, timer caps, etc
# Note that this works for the platforms that we have at the moment, and may
# need to be increased in the future
set(KernelRootCNodeSizeBits 13 CACHE INTERNAL "")

# Set our custom domain schedule
sel4_import_kernel()

# Elfloader settings that correspond to how Data61 sets its boards up.
ApplyData61ElfLoaderSettings(${KernelPlatform} ${KernelSel4Arch})
elfloader_import_project()

# Set up runtime
include(environment_flags)
add_default_compilation_options()

find_libgcc_files()

set(CRTObjFiles "${CMAKE_BINARY_DIR}/lib/crt0.o ${CMAKE_BINARY_DIR}/lib/crti.o ${CRTBeginFile}")
set(FinObjFiles "${CRTEndFile} ${CMAKE_BINARY_DIR}/lib/crtn.o")

# libgcc has dependencies implemented by libc and so we use link groups to resolve these.
# This seems to be the same behaviour gcc has when building static binaries.
set(common_link_string "<LINK_FLAGS> ${CRTObjFiles} <OBJECTS> -Wl,--start-group \
    ${libgcc} <LINK_LIBRARIES> -Wl,--end-group ${FinObjFiles} -o <TARGET>")
set(
    CMAKE_C_LINK_EXECUTABLE
    "<CMAKE_C_COMPILER>  <FLAGS> <CMAKE_C_LINK_FLAGS> ${common_link_string}"
)
set(
    CMAKE_CXX_LINK_EXECUTABLE
    "<CMAKE_CXX_COMPILER>  <FLAGS> <CMAKE_CXX_LINK_FLAGS> ${common_link_string}"
)
set(
    CMAKE_ASM_LINK_EXECUTABLE
    "<CMAKE_ASM_COMPILER>  <FLAGS> <CMAKE_ASM_LINK_FLAGS> ${common_link_string}"
)

# We want to check what we can set the -mfloat-abi to on arm and if that matches what is requested
add_fpu_compilation_options()
# Now all platform compilation flags have been set, we can check the compiler against flags
include(check_arch_compiler)
check_arch_compiler()

sel4_import_libsel4()
sel4runtime_import_project()
util_libs_import_libraries()

# Define the executable
add_executable(example EXCLUDE_FROM_ALL roottask.c rt.c fail.c debug.c c.c)
target_link_libraries(example PUBLIC sel4_autoconf sel4 sel4runtime)

# Set this image as the rootserver
include(rootserver)
DeclareRootserver(example)
