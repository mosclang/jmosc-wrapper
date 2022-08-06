//
// Created by Mahamadou DOUMBIA [OML DSI] on 22/02/2022.
//

#ifndef MOSC_DUMARE_H
#define MOSC_DUMARE_H

#include "../api/msc.h"
#include "../common/common.h"


MSCExternMethodFn MSCMirrorBindExternMethod(MVM* vm,
                                                const char* className,
                                                bool isStatic,
                                                const char* signature);
void mirrorClassMirrorHasMethod(MVM *vm);
void mirrorClassMirrorMethodNames(MVM *vm);
void mirrorMethodMirrorBoundToClass_(MVM *vm);
void mirrorMethodMirrorModule_(MVM *vm);
void mirrorMethodMirrorSignature_(MVM *vm);
void mirrorFiberMirrorLineAt(MVM *vm);
void mirrorFiberMirrorMethodAt(MVM *vm);
void mirrorFiberMirrorStackFramesCount(MVM *vm);
void mirrorModuleMirrorFromName_(MVM *vm);
void mirrorModuleMirrorName_(MVM *vm);
void mirrorObjectMirrorCanInvoke(MVM *vm);


#endif //MOSC_DUMARE_H
