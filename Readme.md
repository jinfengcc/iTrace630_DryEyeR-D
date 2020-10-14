# Environment

* Visual Studio 2019
* Microsoft vcpkg library packager (https://github.com/microsoft/vcpkg)
* Google Test for Unit testing
* Google Benchmark for algorithm benchmarking

# VCPKG Configuration

The following libaries are used and managed with vcpkg

* benchmark:x86-windows
* boost:x86-windows
* bzip2:x86-windows
* dcmtk:x86-windows
* fmt:x86-windows
* gtest:x86-windows
* ms-gsl:x86-windows
* nlohmann-json:x86-windows
* opencv4:x86-windows
* opengl:x86-windows
* wil:x86-windows
* wtl:x64-windows
* zlib:x86-windows

# Tools

* Markdown editor Visual Studio extension (https://github.com/madskristensen/MarkdownEditor)
* Clang format is used for formatting (https://clang.llvm.org/docs/ClangFormat.html)
* Visual studio extension for image debugging (https://marketplace.visualstudio.com/items?itemName=VisualCPPTeam.ImageWatch2019)
* Homegrown TkTraceWin for logging (included in the tools folder)
* Homegrown ImgDbgWin for image debugging (included in the tools folder). See [here](./src/libs/CalcLib/Image/ImgDbgWin.h) 