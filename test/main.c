#include <stdio.h>
#include "../src/java_mosc.h"
#include "../deps/mosc/api/msc.h"

void reporter(const char* method, const char* key, const char* value) {

}
const char* loadModule(const char * name) {
    printf("LoadingC:: %s\n", name);
    return "";
}
int main() {
    int i = 5;
    MSCConfig config;
    JMVMConfig jmvmConfig;
    JMVM *jmvm = NULL;
    while (i-- > 0) {
        MSCInitConfig(&config);
        initJVMConfig(&jmvmConfig);
        jmvmConfig.hostLoadModuleLoader = loadModule;
        jmvm = newVM(&config, &jmvmConfig);
        interpret(jmvm, "<ii>", "nani \"jvm\"");
        freeVM(jmvm);
    }

   printf("Done");
    return 0;
}