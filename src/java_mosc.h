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


typedef MSCExternClassMethods* (*BindExternClassFn)(
        MVM *vm, const char *module, const char *className);

typedef struct JMVMConfig {
    BindExternClassFn  hostExternClassLoader;
    MSCBindExternMethodFn  hostExternMethodLoader;
    JWrapperModuleResolver  hostLoadModuleLoader;
    JWrapperReporter reporter;
} JMVMConfig;

typedef struct {
    MVM* vm;
    MSCConfig* config;
    JMVMConfig* jmvmConfig;
} JMVM;


typedef struct {
    void* handle;
} JVMClass;


JMVM* newVM(MSCConfig * config,  JMVMConfig *jmvmConfig);
VMReturnCode interpret(JMVM* jmvm, const char * module, const char* source);
void initJVMConfig(JMVMConfig * config);
void freeVM(JMVM* jmvm);
void setJVMModuleResolver(JMVM *jmvm, MSCLoadModuleFn  resolver);
void MSCSetSlotNewJVMClass(MVM *vm);
// void initMethods(MSCExternClassMethods methods);
#endif