//
// Created by Mahamadou DOUMBIA [OML DSI] on 28/02/2022.
//

#include "nafamaw.h"
#include "_source.msc.inc"
#include "strings.h"
#include "json.h"
#include "dumare.h"

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

ModuleRegistry nafamawModules[] =
        {
                MODULE(json)
                                CLASS(JSONStream)
                                                METHOD("streamBegin_(_)", jsonStreamBegin)
                                                METHOD("streamEnd()", jsonStreamEnd)
                                                METHOD("next", jsonStreamNext)
                                                METHOD("value", jsonStreamValue)
                                                METHOD("lineno", jsonStreamLineNumber)
                                                METHOD("pos", jsonStreamPos)
                                                METHOD("errorMessage", jsonStreamErrorMessage)
                                END_CLASS
                END_MODULE
                MODULE(dumare)
                        CLASS(Dumare)
                        END_CLASS
                        CLASS(FenDumare)
                                        STATIC_METHOD("beeseKaWele(_,_)", mirrorObjectMirrorCanInvoke)
                        END_CLASS
                        CLASS(KuluDumare)
                                        STATIC_METHOD("hasMethod(_,_)", mirrorClassMirrorHasMethod)
                                        STATIC_METHOD("methodNames(_)", mirrorClassMirrorMethodNames)
                        END_CLASS
                        CLASS(TiidenDumare)
                                        STATIC_METHOD("boundToClass_(_)", mirrorMethodMirrorBoundToClass_)
                                        STATIC_METHOD("module_(_)", mirrorMethodMirrorModule_)
                                        STATIC_METHOD("signature_(_)", mirrorMethodMirrorSignature_)
                        END_CLASS
                        CLASS(DjuruDumare)
                                        STATIC_METHOD("methodAt_(_,_)", mirrorFiberMirrorMethodAt)
                                        STATIC_METHOD("lineAt_(_,_)", mirrorFiberMirrorLineAt)
                                        STATIC_METHOD("stackFramesCount_(_)", mirrorFiberMirrorStackFramesCount)
                        END_CLASS
                        CLASS(KabilaDumare)
                                        STATIC_METHOD("fromName_(_)", mirrorModuleMirrorFromName_)
                                        STATIC_METHOD("name_(_)", mirrorModuleMirrorName_)
                        END_CLASS
                        CLASS(StackTrace)
                        END_CLASS
                        CLASS(StackTraceFrame)
                        END_CLASS
                END_MODULE
                MODULE(network)
                                CLASS(Http)
                                END_CLASS
                                CLASS(WebSocket)
                                END_CLASS
                END_MODULE
                MODULE(nafamaw)
                                CLASS(Sebenw)
                                                STATIC_METHOD("up_(_)", stringsUpcase)
                                                STATIC_METHOD("down_(_)", stringsDowncase)
                                                STATIC_METHOD("globMatch_(_,_,_,_)", stringsGlobMatch)
                                END_CLASS
                END_MODULE
                SENTINEL_MODULE
        };

ModuleRegistry *returnRegistry() {
    return nafamawModules;
}

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