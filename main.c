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

    continueRunAsm = validateProgramArgs(argc, argv);
    if (continueRunAsm) {
        continueRunAsm = preAssembler(argc, argv);
        if (continueRunAsm) {
            assembler(argc, argv);
        }
    }
}
