#!/usr/bin/env python
# coding: utf-8

import argparse
import glob
import os.path
import re

# The source for the Mosc packages that are built into the VM or CLI are turned
# include C string literals. This way they can be compiled directly into the
# code so that file IO is not needed to find and read them.
#
# These string literals are stored in files with a ".msc.inc" extension and
# #included directly by other source files. This generates a ".msc.inc" file
# given a ".msc" module.

PREAMBLE = """// Generated automatically from {0}. Do not edit.
static const char* {1}ModuleSource =
{2};
"""

def mosc_to_c_string(input_path, mosc_source_lines, module):
    mosc_source = ""
    # cut off blank lines at the bottom
    while (mosc_source_lines[-1].strip()==""):
        mosc_source_lines.pop()
    for line in mosc_source_lines:
        line = line.replace('\\','\\\\')
        line = line.replace('"', "\\\"")
        line = line.replace("\n", "\\n")
        mosc_source += '"' + line + '"\n'

    mosc_source = mosc_source.strip()

    return PREAMBLE.format("src/packages/*.msc", module, mosc_source)

def process_file(path, modules):
    infile = os.path.basename(path)
    outfile = path + ".inc"
    # print("{} => {}").format(path.replace("src/",""), outfile)

    with open(path, "r") as f:
        mosc_source_lines = f.readlines() + ["\n\n"]

    first = mosc_source_lines[0]
    m = re.search(r'##package=(.*)',first)
    if (m):
        moduleNames = m.group(1).split(",")
    else:
        moduleNames = [os.path.splitext(infile)[0]]

    for module in moduleNames:
        module = module.replace("opt_", "")
        modules[module] = modules.get(module,[])
        modules[module].extend(mosc_source_lines)


module_files = {}

def main():
    files = glob.glob("src/packages/*.msc")
    with open("src/packages/_source.msc.inc", "w") as f:
        modules = {}
        for file in files:
            process_file(file, modules)
        for (module,lines) in modules.items():
            source = mosc_to_c_string("", lines, module)
            f.write(source + "\n")

main()