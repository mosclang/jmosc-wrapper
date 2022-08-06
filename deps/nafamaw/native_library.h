//
// Created by Mahamadou DOUMBIA [OML DSI] on 21/02/2022.
//

#ifndef MOSC_NATIVE_LIBRARY_H
#define MOSC_NATIVE_LIBRARY_H


// The maximum number of foreign methods a single class defines. Ideally, we
// would use variable-length arrays for each class in the table below, but
// C++98 doesn't have any easy syntax for nested global static data, so we
// just use worst-case fixed-size arrays instead.
//
// If you add a new method to the longest class below, make sure to bump this.
// Note that it also includes an extra slot for the sentinel value indicating
// the end of the list.
#define MAX_METHODS_PER_CLASS 20

// The maximum number of foreign classes a single built-in module defines.
//
// If you add a new class to the largest module below, make sure to bump this.
// Note that it also includes an extra slot for the sentinel value indicating
// the end of the list.
#define MAX_CLASSES_PER_MODULE 20

#define MAX_MODULES_PER_LIBRARY 20
#define MAX_LIBRARIES 20

typedef struct
{
    bool isStatic;
    const char* signature;
    MSCExternMethodFn method;
} MethodRegistry;

// Describes one class in a built-in module.
typedef struct
{
    const char* name;

    MethodRegistry methods[MAX_METHODS_PER_CLASS];
} ClassRegistry;

// Describes one built-in module.
typedef struct
{
    // The name of the module.
    const char* name;

    // Pointer to the string containing the source code of the module. We use a
    // pointer here because the string variable itself is not a constant
    // expression so can't be used in the initializer below.
    const char **source;

    ClassRegistry classes[MAX_CLASSES_PER_MODULE];
} ModuleRegistry;

typedef struct
{
    const char* name;

    ModuleRegistry (*modules)[MAX_MODULES_PER_LIBRARY];
} PackageRegistry;

void registerPackage(const char *name, ModuleRegistry *registry);

typedef ModuleRegistry* (*NativeProvider)();



#endif //MOSC_NATIVE_LIBRARY_H
