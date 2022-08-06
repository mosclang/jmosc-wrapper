#include "msc.h"
#include "native_library.h"

#ifndef JAVAMOSC_LIBRARY_H
#define JAVAMOSC_LIBRARY_H


typedef  enum  {
    VM_SUCCESS,
    VM_ERROR
} VMReturnCode;


struct VMReturn {
    VMReturnCode code;
    const char* data;
};
// Displays a string of text to the user.
typedef const char* (*JWrapperModuleResolver)(const char * name);
typedef void (*JWrapperReporter)(MVM* vm, const char *key);
typedef struct JMVMConfig {
    MSCBindExternClassFn  hostExternClassLoader;
    MSCBindExternMethodFn  hostExternMethodLoader;
    JWrapperModuleResolver  hostLoadModuleLoader;
    JWrapperReporter reporter;
} JMVMConfig;

typedef struct {
    MVM* vm;
    MSCConfig* config;
    JMVMConfig* jmvmConfig;
} JMVM;



JMVM* newVM(MSCConfig * config,  JMVMConfig *jmvmConfig);
VMReturnCode interpret(JMVM* jmvm, const char * module, const char* source);
void initJVMConfig(JMVMConfig * config);
void freeVM(JMVM* jmvm);
void setJVMModuleResolver(JMVM *jmvm, MSCLoadModuleFn  resolver);
#endif