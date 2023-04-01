#include <string.h>
#include <stdlib.h>
#include <msc.h>

#include "java_mosc.h"

#include "nafamaw.h"

#include "jwrapper.msc.inc"
#include "ensure.msc.inc"

typedef struct DHello {

} DHello;
static void allocateDHello(MVM *vm) {
    MSCSetSlotNewExtern(vm, 0, 0, sizeof(JVMClass));
}
static void _jwrapperReport(MVM *vm, int dataType) {
    JMVM *underlinedVM = MSCGetUserData(vm);
    if (underlinedVM == NULL) {
        return;
    }
    const char *key = MSCGetSlotString(vm, 1);
    // printf("Ket::: %s", key);
    // const char *value = MSCGetSlotString(vm, 2);/// why ?? I don't know but its required
    underlinedVM->jmvmConfig->reporter(vm, key, dataType);
    MSCSetSlotBool(vm, 0, true);
}
static void jwrapperReport(MVM *vm) {
    _jwrapperReport(vm, 0);
}
static void jwrapperReportMap(MVM *vm) {
    _jwrapperReport(vm, 1);
}
static void jwrapperReportList(MVM *vm) {
    _jwrapperReport(vm, 2);
}



// To locate foreign classes and modules, we build a big directory for them in
// static data. The nested collection initializer syntax gets pretty noisy, so
// define a couple of macros to make it easier.
#define SENTINEL_METHOD { false, NULL, NULL }
#define SENTINEL_CLASS { NULL, { SENTINEL_METHOD } }
#define SENTINEL_MODULE {NULL, NULL, { SENTINEL_CLASS } }

#define NAMED_MODULE(name, identifier) { #name, &identifier##ModuleSource, {
#define MODULE(name) { #name, &name##ModuleSource, {
#define END_MODULE SENTINEL_CLASS } },

#define CLASS(name) { #name, {
#define END_CLASS SENTINEL_METHOD } },

#define METHOD(signature, fn) { false, signature, fn },
#define STATIC_METHOD(signature, fn) { true, signature, fn },
#define ALLOCATE(fn) { true, "<allocate>", (MSCExternMethodFn)fn },
#define FINALIZE(fn) { true, "<finalize>", (MSCExternMethodFn)fn },

static ModuleRegistry wrapperCorePackage[] =
        {
                NAMED_MODULE(java, jwrapper)
                                CLASS(JWrapper)
                                                STATIC_METHOD("report_(_,_)", jwrapperReport)
                                                STATIC_METHOD("reportMap_(_,_)", jwrapperReportMap)
                                                STATIC_METHOD("reportList_(_,_)", jwrapperReportList)
                                                // STATIC_METHOD("status_(_)", jwrapperStatus)
                                END_CLASS
                                CLASS(DHello)
                                ALLOCATE(allocateDHello)
                                END_CLASS
                END_MODULE
                MODULE(ensure)
                END_MODULE
                SENTINEL_MODULE
        };
#undef SENTINEL_METHOD
#undef SENTINEL_CLASS
#undef SENTINEL_MODULE
#undef NAMED_MODULE
#undef MODULE
#undef END_MODULE
#undef CLASS
#undef END_CLASS
#undef METHOD
#undef STATIC_METHOD
#undef FINALIZE

static PackageRegistry packages[MAX_LIBRARIES] = {
        {"core",    (ModuleRegistry (*)[MAX_MODULES_PER_LIBRARY]) &wrapperCorePackage},
        {"nafamaw", (ModuleRegistry (*)[MAX_MODULES_PER_LIBRARY]) &nafamawModules},
        {NULL, NULL}
};


// Looks for a built-in module with [name].
//
// Returns the BuildInModule for it or NULL if not found.
static ModuleRegistry *findModule(const char *name) {
    for (int j = 0; packages[j].name != NULL; j++) {
        ModuleRegistry *modules = &(*packages[j].modules)[0];
        for (int i = 0; modules[i].name != NULL; i++) {
            if (strcmp(name, modules[i].name) == 0) return &modules[i];
        }
    }

    return NULL;
}

// Looks for a class with [name] in [module].
static ClassRegistry *findClass(ModuleRegistry *module, const char *name) {
    for (int i = 0; module->classes[i].name != NULL; i++) {
        if (strcmp(name, module->classes[i].name) == 0) return &module->classes[i];
    }

    return NULL;
}

// Looks for a method with [signature] in [clas].
static MSCExternMethodFn findMethod(ClassRegistry *clas,
                                    bool isStatic, const char *signature) {
    for (int i = 0; clas->methods[i].signature != NULL; i++) {
        MethodRegistry *method = &clas->methods[i];
        if (isStatic == method->isStatic &&
            strcmp(signature, method->signature) == 0) {
            return method->method;
        }
    }

    return NULL;
}

void loadModuleComplete(MVM *vm, const char *name, struct MSCLoadModuleResult result) {
    if (result.source == NULL) return;

    free((void *) result.source);
}

static MSCLoadModuleResult loadBuiltInModule(const char *name) {
    MSCLoadModuleResult result = {0};
    ModuleRegistry *module = findModule(name);
    if (module == NULL) return result;

    size_t length = strlen(*module->source);
    char *copy = (char *) malloc(length + 1);
    memcpy(copy, *module->source, length + 1);

    result.onComplete = loadModuleComplete;
    result.source = copy;

    return result;
}

static MSCExternMethodFn bindBuiltInExternMethod(
        MVM *vm, const char *moduleName, const char *className, bool isStatic,
        const char *signature) {
    JMVM *underlinedVM = MSCGetUserData(vm);
    // TODO: Assert instead of return NULL?
    ModuleRegistry *module = findModule(moduleName);
    if (module == NULL) {
        if (underlinedVM->jmvmConfig->hostExternMethodLoader != NULL) {
            return underlinedVM->jmvmConfig->hostExternMethodLoader(vm, moduleName, className, isStatic, signature);
        }
        return NULL;
    }

    ClassRegistry *clas = findClass(module, className);
    if (clas == NULL) return NULL;

    return findMethod(clas, isStatic, signature);
}

static MSCExternClassMethods bindBuiltInExternClass(
        MVM *vm, const char *moduleName, const char *className) {
    MSCExternClassMethods methods = {NULL, NULL};
    JMVM *underlinedVM = MSCGetUserData(vm);
    ModuleRegistry *module = findModule(moduleName);
    if (module == NULL) {
        if (underlinedVM->jmvmConfig->hostExternClassLoader != NULL) {
            MSCExternClassMethods* meths = underlinedVM->jmvmConfig->hostExternClassLoader(vm, moduleName, className);
            // methods.finalize = NULL;
            methods.allocate = meths->allocate;
            methods.finalize = meths->finalize;
        }
        return methods;
    }
    ClassRegistry *clas = findClass(module, className);
    if (clas == NULL) return methods;

    methods.allocate = findMethod(clas, true, "<allocate>");
    methods.finalize = (MSCFinalizerFn) findMethod(clas, true, "<finalize>");

    return methods;
}


static MSCLoadModuleResult jwloadModulFn(MVM *vm, const char *name) {
    JMVM *underlinedVM = MSCGetUserData(vm);
    MSCLoadModuleResult res = loadBuiltInModule(name);

    if (res.source == NULL && underlinedVM->jmvmConfig->hostLoadModuleLoader != NULL) {
        // try the host loading
        res.source = underlinedVM->jmvmConfig->hostLoadModuleLoader(name);
    }
    return res;
}

void freeVM(JMVM *jmvm) {
    if (jmvm != NULL && jmvm->vm != NULL) {
        MSCFreeVM(jmvm->vm);
        jmvm->vm = NULL;
        jmvm->jmvmConfig->reporter = NULL;
        jmvm->jmvmConfig->hostLoadModuleLoader = NULL;
        jmvm->jmvmConfig->hostExternMethodLoader = NULL;
        jmvm->jmvmConfig->hostExternClassLoader = NULL;
        free(jmvm);
    }
}

void setJVMModuleResolver(JMVM *jmvm, MSCLoadModuleFn resolver) {
    jmvm->jmvmConfig->hostLoadModuleLoader = resolver;
}
void MSCSetSlotNewJVMClass(MVM *vm) {

    MSCSetSlotNewExtern(vm, 0, 0, sizeof(JVMClass));
    // return handler;
}


JMVM *newVM(MSCConfig *config, JMVMConfig *jmvmConfig) {
    JMVM *jmvm = malloc(sizeof(JMVM));
    jmvm->config = config;


    jmvm->config->initialHeapSize = 1024 * 1024 * 10;
    jmvm->config->bindExternMethodFn = bindBuiltInExternMethod;
    jmvm->config->bindExternClassFn = bindBuiltInExternClass;
    jmvm->config->loadModuleFn = jwloadModulFn;
    jmvm->vm = MSCNewVM(jmvm->config);
    jmvm->jmvmConfig = jmvmConfig;
    // return globalVM;
    MSCSetUserData(jmvm->vm, jmvm);
    // MSCInterpret(jmvm->vm, "<init>", "nin _JINPUT_ = {}");
    // MSCSetSlotNewMap(jmvm->vm, 0);
    return jmvm;
}

VMReturnCode interpret(JMVM *jmvm, const char *module, const char *source) {
    if (jmvm == NULL || jmvm->vm == NULL) {
        return VM_ERROR;
    }
    MSCInterpretResult result = MSCInterpret(jmvm->vm, module, source);
    if (result == RESULT_SUCCESS) {
        return VM_SUCCESS;
    }
    return VM_ERROR;
}

void initJVMConfig(JMVMConfig *config) {
    config->hostExternClassLoader = NULL;
    config->hostExternMethodLoader = NULL;
    config->hostLoadModuleLoader = NULL;
    config->reporter = NULL;
}