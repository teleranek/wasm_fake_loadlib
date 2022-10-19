#include "emscripten.h"

EMSCRIPTEN_KEEPALIVE
extern "C" int add_1(int number) {
    return number + 1;
}

EMSCRIPTEN_KEEPALIVE
extern "C" int add_666(int number) {
    return number + 1;
}
