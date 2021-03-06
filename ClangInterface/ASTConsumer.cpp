#include "ASTConsumer.hpp"
#include "FunctionInfo.h"

#include "debug.h"

#include <iostream>

using clang::CompilerInstance;
using clang::TargetOptions;
using clang::TargetInfo;
using clang::FileEntry;
using clang::Token;
using clang::ASTContext;
using clang::ASTConsumer;
using clang::Parser;
using clang::Preprocessor;
using clang::SourceManager;
using clang::SourceLocation;
using clang::FunctionDecl;
/// from TrackMacro.hpp
using clang::TrackMacro;

std::ostream& operator<<(std::ostream& os,const ParsedDeclInfo& inf)
{
  os<<inf.start_line;
  os<<"\t"<<inf.end_line;
  return os;
}

static void set_package_specific_options(clang::PreprocessorOptions& PPOpts)
{
    (void)PPOpts;
    ///PPOpts.UsePredefines = false;
    ///////////////////////////////////////////
    /// hiphop specific options
    //PPOpts.addMacroDef("NO_TCMALLOC");
    /* wxwidgets specific
    PPOpts.addMacroDef("wxUSE_ANY");
    PPOpts.addMacroDef("wxUSE_CONSOLE_EVENTLOOP");
    PPOpts.addMacroDef("wxUSE_FILE_HISTORY");
    PPOpts.addMacroDef("wxUSE_FSWATCHER");
    PPOpts.addMacroDef("wxUSE_IPV6");
    PPOpts.addMacroDef("wxUSE_XLOCALE");*/
    //////////////////////////////////////////
}

static void set_package_specific_paths(clang::HeaderSearchOptions& HSOpts)
{
  (void)HSOpts;
  /// clang specific include
  /// it contains headers with all the standard(platform-specific) definitions etc.
  /*
    HSOpts.AddPath("/home/hiraditya/Documents/llvm/install/lib/clang/3.2/include",
          clang::frontend::Angled,
          false,  false,  false);
  /// General includes
    HSOpts.AddPath("/usr/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/linux",
          clang::frontend::Angled,
          false,  false,  false);

    HSOpts.AddPath("/usr/include/x86_64-linux-gnu",
          clang::frontend::Angled,
          false,  false,  false);

  //only in ix86 computers
    HSOpts.AddPath("/usr/include/i386-linux-gnu",
          clang::frontend::Angled,
          false,  false,  false);

  /// c++ includes
    HSOpts.AddPath("/usr/include/c++/4.6",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/c++/4.6/tr1",
          clang::frontend::Angled,
          false,  false,  false);

  /// c++ platform specific includes

    HSOpts.AddPath("/usr/include/c++/4.6/x86_64-linux-gnu",
          clang::frontend::Angled,
          false,  false,  false);

    HSOpts.AddPath("/usr/include/c++/4.6/x86_64-linux-gnu/bits",
          clang::frontend::Angled,
          false,  false,  false);
  */
  /*only in ix86 computers
  /// c++ platform specific includes
    HSOpts.AddPath("/usr/include/c++/4.6/i686-linux-gnu",
          clang::frontend::Angled,
          false,  false,  false);

    HSOpts.AddPath("/usr/include/c++/4.6/i686-linux-gnu/64",
          clang::frontend::Angled,
          false,  false,  false);

    HSOpts.AddPath("/usr/include/c++/4.6/backward",
          clang::frontend::Angled,
          false,  false,  false);

    HSOpts.AddPath("/usr/include/c++/4.6/x86_64-linux-gnu/backward",
          clang::frontend::Angled,
          false,  false,  false);

    HSOpts.AddPath("/usr/include/c++/4.6/debug",
          clang::frontend::Angled,
          false,  false,  false);

    HSOpts.AddPath("/usr/include/c++/4.6/ext",
          clang::frontend::Angled,
          false,  false,  false);

  ///for local libs e.g. boost
    HSOpts.AddPath("/usr/local/include",
          clang::frontend::Angled,
          false,  false,  false);

    HSOpts.AddPath("/usr/lib/gcc/x86_64-linux-gnu/4.6",
          clang::frontend::Angled,
          false,  false,  false);
  */
  /*only in ix86 computers
  /// gcc specific includes
    HSOpts.AddPath("/usr/lib/gcc/i686-linux-gnu/4.6.1",
          clang::frontend::Angled,
          false,  false,  false);

  */
  /*
  //cryptopp specific
    HSOpts.AddPath("/media/space/packages/cryptopp",
          clang::frontend::Angled,
          false,  false,  false);
  */
  /*
  /// wx-widgets specific
    HSOpts.AddPath("/media/space/packages/wxWidgets-2.9.3/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/lib/wx/include/base-unicode-release-2.8",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/glib-2.0",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/lib/i386-linux-gnu/glib-2.0/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/gtk-3.0",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/pango-1.0",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/cairo",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/gdk-pixbuf-2.0",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/atk-1.0",
          clang::frontend::Angled,
          false,  false,  false);
  */
  /*
  /// ace specific
    HSOpts.AddPath("/home/hiraditya/Downloads/ACE_wrappers",
          clang::frontend::Angled,//in ace local files are in quotes
          false,  false,  false);
  */


  /* poco specific
    HSOpts.AddPath("/home/hiraditya/Downloads/poco-1.4.3p1/XML/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/poco-1.4.3p1/Net/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/poco-1.4.3p1/Foundation/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/poco-1.4.3p1/Util/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/poco-1.4.3p1/CppUnit/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/poco-1.4.3p1/CppUnit/WinTestRunner/include",
          clang::frontend::Angled,
          false,  false,  false);
  */
  /*
  /// torcs specific
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/linux",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/interfaces",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/modules/graphic/ssggraph",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/modules/simu/simuv2",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/modules/simu/simuv2/SOLID-2.0/include",
          clang::frontend::Angled,
          false,  false,  false);

    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/modules/telemetry",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/modules/track",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/libs/client",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/libs/confscreens",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/libs/learning",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/libs/tmath",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/libs",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/libs/musicplayer",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/libs/portability",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/libs/raceengineclient",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/libs/racescreens",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/libs/robottools",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/libs/tgf",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/libs/tgfclient",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/torcs-1.3.3/src/libs/txml",
          clang::frontend::Angled,
          false,  false,  false);
  */
  /* scintilla specific
    HSOpts.AddPath("/usr/include/glib-2.0",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/lib/i386-linux-gnu/glib-2.0/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/gtk-3.0",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/pango-1.0",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/cairo",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/scintilla/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/scintilla/src",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/scintilla/lexlib",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/gdk-pixbuf-2.0",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/atk-1.0",
          clang::frontend::Angled,
          false,  false,  false);
  */
  /*
  /// hiphop specific
    HSOpts.AddPath("/home/hiraditya/Downloads/hiphop-php/install/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/hiphop-php/src",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/hiphop-php/src/system/gen",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/hiphop-php/src/third_party/timelib",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/hiphop-php/src/third_party/xhp",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/hiphop-php/src/third_party/xhp/xhp",
          clang::frontend::Angled,
          false,  false,  false);

    HSOpts.AddPath("/home/hiraditya/Downloads/gperftools-2.0/src",
          clang::frontend::Angled,
          false,  false,  false);

    HSOpts.AddPath("/home/hiraditya/Downloads/libevent-1.4.13-stable/build/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/curl-7.20.0/build/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/curl-7.20.0/build/lib",
          clang::frontend::Angled,
          false,  false,  false);

    HSOpts.AddPath("/home/hiraditya/Downloads/jemalloc-2.2.5/build/include",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/libmbfl",
          clang::frontend::Angled,
          false,  false,  false);

  ///for xml parsing
    HSOpts.AddPath("/usr/include/libxml2",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/php5/main",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/php5/Zend",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/php5/ext",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/php5/TSRM",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/usr/include/c-client",
          clang::frontend::Angled,
          false,  false,  false);
  */
  /* p7zip specific
    HSOpts.AddPath("/home/hiraditya/Downloads/p7zip_9.20.1/CPP/include_windows",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/p7zip_9.20.1/CPP/Windows",
          clang::frontend::Angled,
          false,  false,  false);
    HSOpts.AddPath("/home/hiraditya/Downloads/p7zip_9.20.1/CPP/Common",
          clang::frontend::Angled,
          false,  false,  false);
  */
}

bool MyASTConsumer::HandleTopLevelDecl(clang::DeclGroupRef d)
{
  int count = 0;
  using namespace clang;
  DeclGroupRef::iterator it;
  for( it = d.begin(); it != d.end(); it++)
  {
    count++;

    FunctionDecl *fd = llvm::dyn_cast<clang::FunctionDecl>(*it);
    /// can be extended for class declaration
    if(fd){
      PrintSourceLocation(fd);
    }
  }
return true;
}

/******************************************************************************
 *
 *****************************************************************************/
int MyASTConsumer::InitializeCI(CompilerInstance& ci,
                                std::vector<std::string> const& search_paths)
{
    pci = &ci;
    DEBUG_ASTCONSUMER(dbgs() << "\n\nSearch paths output from ASTConsumer:\n"
                             << search_paths;);
    ci.createDiagnostics(nullptr, true);
    clang::CompilerInvocation *Invocation = new clang::CompilerInvocation;
    ci.setInvocation(Invocation);

    // Configure TargetOptions.
    //TargetOptions &to = ci.getInvocation().getTargetOpts();
    //to.Triple = llvm::sys::getDefaultTargetTriple();
    auto to = std::make_shared<TargetOptions>();
    to->Triple = llvm::sys::getDefaultTargetTriple();

    /// set the language to c++98
    ci.getInvocation().setLangDefaults(ci.getLangOpts(),
                                       clang::InputKind::IK_CXX,
                                       clang::LangStandard::lang_cxx11);

    DEBUG_ASTCONSUMER(if(ci.getInvocation().getLangOpts()->CPlusPlus)
                        dbgs() << "c++ is defined now";);

    TargetInfo *pti = TargetInfo::CreateTargetInfo(ci.getDiagnostics(),
                                                   to);
                                                   //ci.getInvocation().TargetOpts);

    ci.setTarget(pti);
    ci.createFileManager();
    ci.createSourceManager(ci.getFileManager());
    ci.createPreprocessor(clang::TranslationUnitKind::TU_Complete);

    /// \brief to enable parsing of exceptions by the clang front end
    ci.getLangOpts().CXXExceptions = 1;

    using clang::PreprocessorOptions;
    using clang::HeaderSearchOptions;
    using clang::FrontendOptions;

    PreprocessorOptions& PPOpts = ci.getPreprocessorOpts();

    set_package_specific_options(PPOpts);

    Preprocessor& PP = ci.getPreprocessor();
    HeaderSearchOptions& HSOpts = ci.getHeaderSearchOpts();

    /// use the libc++ library
    HSOpts.UseLibcxx = 1;


    /**
     * \brief Adding the directory path for the compiler to search
     * AddPath (StringRef Path, frontend::IncludeDirGroup Group,
     * bool IsUserSupplied, bool IsFramework, bool IgnoreSysRoot,
     * bool IsInternal=false, bool ImplicitExternC=false)
    */
    std::for_each(search_paths.begin(), search_paths.end(),
                  [&HSOpts](std::string const& search_path) {
                  HSOpts.AddPath(search_path, clang::frontend::Angled,
                                 false, false);
                  });

    // Include header paths for the package.
    set_package_specific_paths(HSOpts);

    const FrontendOptions& FEOpts = ci.getFrontendOpts();
    const auto &PCHHR = ci.getPCHContainerReader();
    ApplyHeaderSearchOptions(PP.getHeaderSearchInfo(), HSOpts,
                             PP.getLangOpts(), PP.getTargetInfo().getTriple());

    PP.getBuiltinInfo().InitializeBuiltins(PP.getIdentifierTable(),
                                           PP.getLangOpts());

    clang::InitializePreprocessor(PP, PPOpts, PCHHR, FEOpts);

    std::unique_ptr<ASTConsumer> Consumer(new MyASTConsumer);
    ci.setASTConsumer(std::move(Consumer));
    ci.createASTContext();

    // Pass the callback function to PP and it will manage the memory.
    track_macro = new clang::TrackMacro;
    track_macro->SetCompilerInstance(&ci);
    PP.addPPCallbacks(std::unique_ptr<clang::PPCallbacks>(track_macro));

    return 0;
}

void MyASTConsumer::DumpContent(std::string const& file_name)
{
  CompilerInstance& ci = *pci;
  current_file = file_name;

  // Kind is C_User for now because I do not know how to set the righ option,
  // this does not matter so much, I think it is only used to selectively
  // emit/ignore compiler warnings.
  clang::SrcMgr::CharacteristicKind Kind = clang::SrcMgr::C_User;

  DEBUG_ASTCONSUMER(dbgs() << "Current file name in AST comsumer is: "
                           << current_file;);
  const FileEntry *pFile = ci.getFileManager().getFile(file_name.c_str());
  SourceManager &SourceMgr = ci.getSourceManager();
  SourceMgr.setMainFileID(SourceMgr.createFileID(pFile, SourceLocation(), Kind));

  // set file and loc parameters for the track_macro callback
  // placing here is important. It should be after the source manager
  // has created fileid for the file to be processed.
  track_macro->SetFileName(current_file);
  ///////////////////////////////////////////////////////////
  ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                            &ci.getPreprocessor());
  clang::ParseAST(ci.getPreprocessor(), this, ci.getASTContext());
  ci.getDiagnosticClient().EndSourceFile();
}

void MyASTConsumer::PrintSourceLocation(SourceManager& sm, SourceLocation loc)
{
    clang::PresumedLoc presumed = sm.getPresumedLoc(loc);
    /// print only when the functions are in the current file
    if(current_file == presumed.getFilename()) {
      std::cout<<"line: "<<presumed.getLine();
      std::cout<<", column: "<<presumed.getColumn();
    }
}

void MyASTConsumer::PrintSourceLocation(FunctionDecl* fd)
{
  clang::CompilerInstance& ci = *pci;
  using namespace clang;
  ParsedDeclInfo inf;
  SourceManager& sm = ci.getSourceManager();
  PresumedLoc presumed = sm.getPresumedLoc(fd->getSourceRange().getBegin());
    /// print only when the functions are in the current file
  if(current_file == presumed.getFilename()) {
/*    std::cout<<"Function declaration with name: "<<fd->getNameInfo().getAsString()<<"\n";
    std::cout<<"Start:\t";
    std::cout<<"line: "<<presumed.getLine();
    std::cout<<", column: "<<presumed.getColumn();
    presumed = sm.getPresumedLoc(fd->getSourceRange().getEnd());
    std::cout<<"\nEnd:\t";
    std::cout<<"line: "<<presumed.getLine();
    std::cout<<", column: "<<presumed.getColumn();*/
    inf.start_line = presumed.getLine();
    presumed = sm.getPresumedLoc(fd->getSourceRange().getEnd());
    inf.end_line = presumed.getLine();
    FunctionInfo[fd->getNameInfo().getAsString()] = inf;
  }
}

void MyASTConsumer::PrintStats()
{
  using namespace general_utilities;
  std::cout<<FunctionInfo;
  track_macro->PrintStats();
}

void MyASTConsumer::VerifyMacroScope(bool use_fast)
{
  if (use_fast)
    track_macro->VerifyMacroScopeFast(FunctionInfo);
  else // This was the initial implementation which is compute intensive.
    track_macro->VerifyMacroScope(FunctionInfo);
}

/// relying on move semantics. returning by value
ASTMacroStat_t MyASTConsumer::GetMacroStat()
{
  return track_macro->GetMacroStat();
}
