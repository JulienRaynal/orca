#include <libdiscord.h>
#include "orka-utils.h"

int main (int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "%s <json-file>\n", argv[0]);
    return 1;
  }

  size_t len = 0;
  char * json = orka_load_whole_file(argv[1], &len);

  discord::channel::embed::dati p;
  discord::channel::embed::dati_init(&p);
  discord::channel::embed::dati_from_json(json, len, &p);
  return 0;
}

