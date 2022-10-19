#include <iostream>
#include <string>
#include <vector>
#include "emscripten.h"
#include "emscripten/val.h"

std::vector<std::string> loadedLibs;

EM_ASYNC_JS(void, loadLibrary, (const char *libname_cpp), {
    const libname = UTF8ToString(libname_cpp);
    // you would require nicer instantiation routine,
    // something similar to what emscripten generates
    // when generating "js" file for .wasm file.
    // (or maybe STANDALONE_WASM compile option suffices?)
    const imports = {
        
    };
    const importObject = {
        wasi_snapshot_preview1: imports,
        env : imports
    };
    const result = await WebAssembly.instantiateStreaming(fetch(libname), importObject);
    window["module_" + libname] = {};
    Object.assign(window["module_" + libname], result.instance.exports);
});

int FakeFakeLoadLibrary(std::string libname) {
    loadLibrary(libname.c_str());
    loadedLibs.push_back(libname);
    return loadedLibs.size() - 1;
}

void* FakeGetProcAddress(int LIB_ID, std::string funcName) {
    std::string libname = loadedLibs[LIB_ID];
    
    emscripten::val func = emscripten::val::global(("module_" + libname).c_str())[funcName];
    int f_ptr = emscripten::val::global("Module").call<int>("addFunction",func);
    return (void*)f_ptr;
}

int ADD_LIB_1_TO_88888;
int ADD_LIB_88889_TO_150000;

int main() {
    typedef int (*PFN_Add)(int number);
    
    ADD_LIB_1_TO_88888 = FakeFakeLoadLibrary("add_lib_1_to_88888.wasm");
    ADD_LIB_88889_TO_150000 = FakeFakeLoadLibrary("add_lib_88889_TO_150000.wasm");
    
    PFN_Add pFN_Add1 = reinterpret_cast<PFN_Add>(FakeGetProcAddress(ADD_LIB_1_TO_88888, "add_1"));
    PFN_Add pFN_AddMillion = reinterpret_cast<PFN_Add>(FakeGetProcAddress(ADD_LIB_88889_TO_150000, "add_100000"));
    int sixsixsix = pFN_Add1(665);
    int anotherExample = pFN_AddMillion(666);
    
    std::cout << "Result of adding 1 to 665 is: " << sixsixsix << std::endl;
    std::cout << "Result of adding 100000 to 666 is: " << anotherExample << std::endl;
}
