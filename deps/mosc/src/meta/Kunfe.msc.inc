// Generated automatically from src/meta/Kunfe.msc. Do not edit.
static const char* KunfeModuleSource =
"dunan kulu Kunfeko {\n"
"  dilan kura() {\n"
"    ale.seed_()\n"
"  }\n"
"\n"
"  dilan kura(seed) {\n"
"    nii seed ye Diat {\n"
"      ale.seed_(seed)\n"
"    } note nii seed ye Tugun {\n"
"      nii seed.laKolon Djuru.tike(\"Sequence cannot be empty.\")\n"
"\n"
"      # TODO: Empty sequence.\n"
"      nin seeds = []\n"
"      seginka seed kono element {\n"
"        nii !(element ye Diat) Djuru.tike(\"Sequence elements must all be numbers.\")\n"
"\n"
"        seeds.aFaraAkan(element)\n"
"        nii seeds.count == 16 atike\n"
"      }\n"
"\n"
"      # Cycle the values to fill in any missing slots.\n"
"      nin i = 0\n"
"      foo (seeds.count < 16) {\n"
"        seeds.aFaraAkan(seeds[i])\n"
"        i = i + 1\n"
"      }\n"
"\n"
"      ale.seed_(\n"
"          seeds[0], seeds[1], seeds[2], seeds[3],\n"
"          seeds[4], seeds[5], seeds[6], seeds[7],\n"
"          seeds[8], seeds[9], seeds[10], seeds[11],\n"
"          seeds[12], seeds[13], seeds[14], seeds[15])\n"
"    } note {\n"
"      Djuru.tike(\"Seed must be a number or a sequence of numbers.\")\n"
"    }\n"
"  }\n"
"\n"
"  dunan seed_()\n"
"  dunan seed_(seed)\n"
"  dunan seed_(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15, n16)\n"
"\n"
"  dunan float()\n"
"  float(end) { ale.float() * end }\n"
"  float(start, end) { ale.float() * (end - start) + start }\n"
"\n"
"  dunan int()\n"
"  int(end) { (ale.float() * end).floor }\n"
"  int(start, end) { (ale.float() * (end - start)).floor + start }\n"
"\n"
"  sample(list) {\n"
"    nii (list.count == 0) Djuru.tike(\"Not enough elements to sample.\")\n"
"    segin niin list[ale.int(list.count)]\n"
"  }\n"
"  sample(list, count) {\n"
"    nii (count > list.count) Djuru.tike(\"Not enough elements to sample.\")\n"
"\n"
"    nin result = []\n"
"\n"
"    # The algorithm described in \"Programming pearls: a sample of brilliance\".\n"
"    # Use a hash map for sample sizes less than 1/4 of the population size and\n"
"    # an array of booleans for larger samples. This simple heuristic improves\n"
"    # performance for large sample sizes as well as reduces memory usage.\n"
"    nii (count * 4 < list.count) {\n"
"      nin picked = {}\n"
"      seginka ((list.count - count...list.count) kono i) {\n"
"        nin index = ale.int(i + 1)\n"
"        nii picked.containsKey(index) index = i\n"
"        picked[index] = tien\n"
"        result.aFaraAkan(list[index])\n"
"      }\n"
"    } note {\n"
"      nin picked = Walan.filled(list.count, galon)\n"
"      seginka ((list.count - count...list.count) kono i) {\n"
"        nin index = ale.int(i + 1)\n"
"        nii (picked[index]) index = i\n"
"        picked[index] = tien\n"
"        result.aFaraAkan(list[index])\n"
"      }\n"
"    }\n"
"\n"
"    segin niin result\n"
"  }\n"
"\n"
"  shuffle(list) {\n"
"    nii (list.laKolon) segin\n"
"\n"
"    # Fisher-Yates shuffle.\n"
"    seginka ((0...list.count - 1) kono i) {\n"
"      nin from = ale.int(i, list.count)\n"
"      nin temp = list[from]\n"
"      list[from] = list[i]\n"
"      list[i] = temp\n"
"    }\n"
"  }\n"
"}\n"
"\n";