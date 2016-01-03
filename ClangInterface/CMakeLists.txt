cmake_minimum_required(VERSION 2.8)
project(ASTConsumer)

include_directories("${CMAKE_SOURCE_DIR}")

add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-D__STDC_FORMAT_MACROS)
add_definitions(-D__STDC_LIMIT_MACROS)
ADD_DEFINITIONS(-D_GNU_SOURCE)
#add_definitions(-DCLANG_AST_DEBUG)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}  "-std=c++11 -fno-rtti -g -Wno-non-virtual-dtor")

link_directories("${CFE_PREFIX_PATH}/lib")

add_library(ASTConsumer SHARED
    TrackMacro.cpp
    ASTConsumer.cpp
  )

target_link_libraries(ASTConsumer
  # Output of llvm-config.
LLVMLTO LLVMObjCARCOpts LLVMLinker LLVMBitWriter LLVMIRReader LLVMMIRParser LLVMAsmParser LLVMLibDriver
LLVMOption LLVMDebugInfoPDB LLVMTableGen LLVMOrcJIT LLVMPasses LLVMipo LLVMVectorize LLVMX86Disassembler
LLVMX86AsmParser LLVMX86CodeGen LLVMSelectionDAG LLVMAsmPrinter LLVMX86Desc LLVMMCDisassembler LLVMX86Info
LLVMX86AsmPrinter LLVMX86Utils LLVMMCJIT LLVMDebugInfoDWARF LLVMLineEditor LLVMInterpreter
LLVMExecutionEngine LLVMRuntimeDyld LLVMCodeGen LLVMTarget LLVMScalarOpts LLVMProfileData LLVMObject
LLVMMCParser LLVMBitReader LLVMInstCombine LLVMInstrumentation LLVMTransformUtils LLVMipa LLVMMC
LLVMAnalysis LLVMCore LLVMSupport
# Clang frontend tools.
clangFrontendTool clangFrontend clangDriver clangSerialization clangCodeGen clangParse clangSema clangAnalysis clangEdit clangAST clangLex clangBasic
# system libraries.
tinfo pthread z dl
  #LLVMOption LLVMSelectionDAG LLVMInterpreter LLVMCodeGen LLVMScalarOpts LLVMInstCombine LLVMTransformUtils LLVMipa LLVMAnalysis LLVMTarget LLVMRuntimeDyld LLVMExecutionEngine LLVMMC LLVMObject LLVMBitReader LLVMCore LLVMSupport
  )
