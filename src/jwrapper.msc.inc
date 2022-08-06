// Generated automatically from src/jwrapper.msc. Do not edit.
static const char* jwrapperModuleSource =
"kabo \"json\" nani JSON\n"
"kulu JWrapper {\n"
"    dunan dialen report_(key, value)\n"
"    dialen report(map) {\n"
"       seginka (map kono entry) {\n"
"           JWrapper.report(entry.key, entry.value.sebenma)\n"
"       }\n"
"     }\n"
"    dialen report(key, value) {\n"
"        nin valueStr = tumamin(value) {\n"
"           ye Wala => JSON.stringify(value)\n"
"           ye Walan => JSON.stringify(value)\n"
"           note => value.sebenma\n"
"       }\n"
"        JWrapper.report_(key, valueStr)\n"
"    }\n"
"}\n";
