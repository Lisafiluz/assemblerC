#include "util/readerUtils.h"
#include "validators/progValidator.h"
#include "service/preAssembler.h"
#include "service/assembler.h"

void runAssembler(int argc, char **argv);

int main(int argc, char **argv) {
    runAssembler(argc, argv);
    return 0;
}

void runAssembler(int argc, char **argv) {
    int continueRunAsm;
    continueRunAsm = 1;

    continueRunAsm = validateProgramArgs(argc, argv);
    if (continueRunAsm) {
        preAssembler(argc, argv);
        assembler(argc, argv);
    }
}
