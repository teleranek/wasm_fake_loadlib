var fs = require('fs');
function generate_lib(start, end, name) {
    var fileContent = [
        '#include "emscripten.h"',
    ];
    for (var i = start; i < end; i++) {
        fileContent.push( 
        `EMSCRIPTEN_KEEPALIVE
extern "C" int add_${i}(int n) { return n + ${i}; }`);
    }
    fs.writeFileSync(name + '.cpp', fileContent.join('\n'));
}

generate_lib(1, 88888, "add_lib_1_to_88888");
generate_lib(88889, 150000, "add_lib_88889_to_150000");
