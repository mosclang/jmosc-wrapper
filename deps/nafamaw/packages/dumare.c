//
// Created by Mahamadou DOUMBIA [OML DSI] on 22/02/2022.
//

#include "dumare.h"



#include <string.h>

#include "../runtime/MVM.h"



static Class *mirrorGetSlotClass(MVM *vm, int slot) {
    Value classVal = *MSCSlotAtUnsafe(vm, slot);
    if (!IS_CLASS(classVal)) return NULL;

    return AS_CLASS(classVal);
}

static Djuru *mirrorGetSlotFiber(MVM *vm, int slot) {
    Value fiberVal = *MSCSlotAtUnsafe(vm, slot);
    if (!IS_FIBER(fiberVal)) return NULL;

    return AS_DJURU(fiberVal);
}

static Closure *mirrorGetSlotClosure(MVM *vm, int slot) {
    Value closureVal = *MSCSlotAtUnsafe(vm, slot);
    if (!IS_CLOSURE(closureVal)) return NULL;

    return AS_CLOSURE(closureVal);
}

static Module *mirrorGetSlotModule(MVM *vm, int slot) {
    Value moduleVal = *MSCSlotAtUnsafe(vm, slot);
    if (!IS_MODULE(moduleVal)) return NULL;

    return AS_MODULE(moduleVal);
}

void mirrorClassMirrorHasMethod(MVM *vm) {
    Class *classObj = mirrorGetSlotClass(vm, 1);
    const char *signature = MSCGetSlotString(vm, 2);

    bool hasMethod = false;
    if (classObj != NULL &&
        signature != NULL) {
        int symbol = MSCSymbolTableFind(&vm->methodNames,
                                        signature, strlen(signature));
        hasMethod = MSCClassGetMethod(vm, classObj, symbol) != NULL;
    }
    MSCSetSlotBool(vm, 0, hasMethod);
}

void mirrorClassMirrorMethodNames(MVM *vm) {
    Class *classObj = mirrorGetSlotClass(vm, 1);

    if (!classObj) {
        MSCSetSlotNull(vm, 0);
        return;
    }

    MSCSetSlotNewList(vm, 0);
    for (size_t symbol = 0; symbol < classObj->methods.count; symbol++) {
        Method *method = MSCClassGetMethod(vm, classObj, symbol);
        if (method == NULL) continue;

        *MSCSlotAtUnsafe(vm, 1) = OBJ_VAL(vm->methodNames.data[symbol]);
        MSCInsertInList(vm, 0, -1, 1);
    }
}

void mirrorFiberMirrorMethodAt(MVM *vm) {
    Djuru *fiber = mirrorGetSlotFiber(vm, 1);
    size_t index = MSCGetSlotDouble(vm, 2);
    CallFrame *frame;

    if (fiber == NULL ||
        (frame = &fiber->frames[index])->closure == NULL) {
        MSCSetSlotNull(vm, 0);
        return;
    }
    *MSCSlotAtUnsafe(vm, 0) = OBJ_VAL(frame->closure);
}

void mirrorFiberMirrorLineAt(MVM *vm) {
    Djuru *fiber = mirrorGetSlotFiber(vm, 1);
    size_t index = MSCGetSlotDouble(vm, 2);
    CallFrame *frame;
    Function *fn;

    if (fiber == NULL ||
        (frame = &fiber->frames[index]) == NULL ||
        (fn = frame->closure->fn) == NULL ||
        fn->debug->sourceLines.data == NULL) {
        MSCSetSlotNull(vm, 0);
        return;
    }

    size_t line = fn->debug->sourceLines.data[frame->ip - fn->code.data - 1];

    MSCSetSlotDouble(vm, 0, line);
}

void mirrorFiberMirrorStackFramesCount(MVM *vm) {
    Djuru *fiber = mirrorGetSlotFiber(vm, 1);

    if (fiber == NULL) {
        MSCSetSlotNull(vm, 0);
        return;
    }

    MSCSetSlotDouble(vm, 0, fiber->numOfFrames);
}


void mirrorMethodMirrorBoundToClass_(MVM *vm) {
    Closure *closureObj = mirrorGetSlotClosure(vm, 1);

    if (!closureObj) {
        MSCSetSlotNull(vm, 0);
        return;
    }

    *MSCSlotAtUnsafe(vm, 0) = OBJ_VAL(closureObj->fn->boundToClass);
}

void mirrorMethodMirrorModule_(MVM *vm) {
    Closure *closureObj = mirrorGetSlotClosure(vm, 1);

    if (!closureObj) {
        MSCSetSlotNull(vm, 0);
        return;
    }

    *MSCSlotAtUnsafe(vm, 0) = OBJ_VAL(closureObj->fn->module);
}

void mirrorMethodMirrorSignature_(MVM *vm) {
    Closure *closureObj = mirrorGetSlotClosure(vm, 1);

    if (!closureObj) {
        MSCSetSlotNull(vm, 0);
        return;
    }

    MSCSetSlotString(vm, 0, closureObj->fn->debug->name);
}

void mirrorModuleMirrorFromName_(MVM *vm) {
    const char *moduleName = MSCGetSlotString(vm, 1);

    if (!moduleName) {
        MSCSetSlotNull(vm, 0);
        return;
    }

    // Special case for "core"
    if (strcmp(moduleName, "core") == 0) {
        MSCSetSlotNull(vm, 1);
    }

    Module *module = MSCGetModule(vm, *MSCSlotAtUnsafe(vm, 1));
    if (module != NULL) {
        *MSCSlotAtUnsafe(vm, 0) = OBJ_VAL(module);
    } else {
        MSCSetSlotNull(vm, 0);
    }
}

void mirrorModuleMirrorName_(MVM *vm) {
    Module *moduleObj = mirrorGetSlotModule(vm, 1);
    if (!moduleObj) {
        MSCSetSlotNull(vm, 0);
        return;
    }

    if (moduleObj->name) {
        *MSCSlotAtUnsafe(vm, 0) = OBJ_VAL(moduleObj->name);
    } else {
        // Special case for "core"
        MSCSetSlotString(vm, 0, "core");
    }
}

void mirrorObjectMirrorCanInvoke(MVM *vm) {
    Class *classObj = MSCGetClassInline(vm, vm->apiStack[1]);
    vm->apiStack[1] = OBJ_VAL(classObj);

    mirrorClassMirrorHasMethod(vm);
}


MSCExternMethodFn MSCMirrorBindExternMethod(MVM *vm,
                                            const char *className,
                                            bool isStatic,
                                            const char *signature) {
    if (strcmp(className, "KuluDumare") == 0) {
        if (isStatic &&
            strcmp(signature, "hasMethod(_,_)") == 0) {
            return mirrorClassMirrorHasMethod;
        }
        if (isStatic &&
            strcmp(signature, "methodNames(_)") == 0) {
            return mirrorClassMirrorMethodNames;
        }
    }
    if (strcmp(className, "DjuruDumare") == 0) {
        if (isStatic &&
            strcmp(signature, "methodAt_(_,_)") == 0) {
            return mirrorFiberMirrorMethodAt;
        }
        if (isStatic &&
            strcmp(signature, "lineAt_(_,_)") == 0) {
            return mirrorFiberMirrorLineAt;
        }
        if (isStatic &&
            strcmp(signature, "stackFramesCount_(_)") == 0) {
            return mirrorFiberMirrorStackFramesCount;
        }
    }
    if (strcmp(className, "TiidenDumare") == 0) {
        if (isStatic &&
            strcmp(signature, "boundToClass_(_)") == 0) {
            return mirrorMethodMirrorBoundToClass_;
        }
        if (isStatic &&
            strcmp(signature, "module_(_)") == 0) {
            return mirrorMethodMirrorModule_;
        }
        if (isStatic &&
            strcmp(signature, "signature_(_)") == 0) {
            return mirrorMethodMirrorSignature_;
        }
    }

    if (strcmp(className, "KabilaDumare") == 0) {
        if (isStatic &&
            strcmp(signature, "fromName_(_)") == 0) {
            return mirrorModuleMirrorFromName_;
        }
        if (isStatic &&
            strcmp(signature, "name_(_)") == 0) {
            return mirrorModuleMirrorName_;
        }
    }

    if (strcmp(className, "FenDumare") == 0) {
        if (isStatic &&
            strcmp(signature, "beeseKaWele(_,_)") == 0) {
            return mirrorObjectMirrorCanInvoke;
        }
    }

    ASSERT(false, "Unknown method.");
    return NULL;
}

