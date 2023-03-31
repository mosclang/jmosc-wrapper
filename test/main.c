#include <stdio.h>
#include <msc.h>
#include "../src/java_mosc.h"
#include "../deps/mosc/api/msc.h"

void reporter(const char* method, const char* key, const char* value) {

}
const char* loadModule(const char * name) {
    printf("LoadingC:: %s\n", name);
    return "";
}
static void print(MVM *_, const char *text) {
    printf("%s", text);
}

static bool errorPrint(MVM *vm, MSCError type, const char *module_name, int line, const char *message) {
    printf("Error at %s > %d: %s\n", module_name, line, message);
    return true;
}
int main() {
    int i = 5;
    MSCConfig config;

    JMVMConfig jmvmConfig;
    JMVM *jmvm = NULL;
    while (i-- > 0) {
        MSCInitConfig(&config);
        initJVMConfig(&jmvmConfig);
        config.writeFn = print;
        config.errorHandler = errorPrint;
        // jmvmConfig.hostLoadModuleLoader = loadModule;
        // jmvmConfig.hostLoadModuleLoader = loadModule;
        jmvm = newVM(&config, &jmvmConfig);
        interpret(jmvm, "<ii>", "kabo \"java\" nani  DHello\n\nDHello.kura()\n");
        freeVM(jmvm);
    }

   printf("Done");
    return 0;
}