CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(ASTConsumer)

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")

ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
ADD_DEFINITIONS(-D__STDC_FORMAT_MACROS)
ADD_DEFINITIONS(-D__STDC_LIMIT_MACROS)
ADD_DEFINITIONS(-D_GNU_SOURCE)
ADD_DEFINITIONS(-DCLANG_AST_DEBUG_PRINT)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}  "-std=c++11 -fno-rtti -g -Wno-non-virtual-dtor")

LINK_DIRECTORIES("${CFE_PREFIX_PATH}/lib")

ADD_LIBRARY(ASTConsumer SHARED
    TrackMacro.cpp
    ASTConsumer.cpp
  )

TARGET_LINK_LIBRARIES(ASTConsumer
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
