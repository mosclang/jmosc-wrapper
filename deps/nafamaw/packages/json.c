//
// Created by Mahamadou DOUMBIA [OML DSI] on 25/02/2022.
//

#include "json.h"
#include "pdjson.h"

json_stream jsonStream[1];

void jsonStreamBegin(MVM *vm) {
    const char *value = MSCGetSlotString(vm, 1);
    json_open_string(jsonStream, value);
    json_set_streaming(jsonStream, 0);
}

void jsonStreamEnd(MVM *vm) {
    json_reset(jsonStream);
    json_close(jsonStream);
}

void jsonStreamValue(MVM *vm) {
    const char *value = json_get_string(jsonStream, 0);
    MSCSetSlotString(vm, 0, value);
}

void jsonStreamErrorMessage(MVM *vm) {
    const char *error = json_get_error(jsonStream);
    if (error) {
        MSCSetSlotString(vm, 0, error);
        return;
    }
    MSCSetSlotString(vm, 0, "");
}

void jsonStreamLineNumber(MVM *vm) {
    MSCSetSlotDouble(vm, 0, json_get_lineno(jsonStream));
}

void jsonStreamPos(MVM *vm) {
    MSCSetSlotDouble(vm, 0, json_get_position(jsonStream));
}

void jsonStreamNext(MVM *vm) {
    enum json_type type = json_next(jsonStream);
    // 0 in the enum seems to be reserved for no more tokens
    if (type > 0) {
        MSCSetSlotDouble(vm, 0, type);
        return;
    }
    MSCSetSlotNull(vm, 0);
}