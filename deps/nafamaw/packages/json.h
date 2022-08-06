//
// Created by Mahamadou DOUMBIA [OML DSI] on 25/02/2022.
//

#ifndef MOSC_NAFAMAW_JSON_H
#define MOSC_NAFAMAW_JSON_H

#include "msc.h"

void jsonStreamBegin(MVM * vm);
void jsonStreamEnd(MVM * vm);
void jsonStreamValue(MVM * vm);
void jsonStreamErrorMessage(MVM * vm);
void jsonStreamLineNumber(MVM * vm);
void jsonStreamPos(MVM * vm);
void jsonStreamNext(MVM * vm);

#endif //MOSC_NAFAMAW_JSON_H
