#include "util/readerUtils.h"
#include "validators/progValidator.h"
#include "service/preAssembler.h"

void runAssembler(int argc, char **argv);

int main(int argc, char **argv) {

    runAssembler(argc, argv);
    return 0;
}

void runAssembler(int argc, char **argv) {
    int continueRunAsm;
    continueRunAsm = 1;
    //assembler x y hello
    continueRunAsm = validateProgramArgs(argc, argv);
    if (continueRunAsm) {
        preAssembler(argc, argv);
//        continueRunAsm = firstTransition(argv);
//        if (continueRunAsm) {
//            continueRunAsm = secondTransition(argv);
//        }
    }
}
