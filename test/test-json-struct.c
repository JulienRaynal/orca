#include <stdio.h>
#include <string.h>
#include "json-struct.c"
#include "json-scanf.h"

int main (int argc, char ** argv)
{
  size_t len = 0;
  char * s;

  char * spec =
    "{ |namespace|: [ |A|, |B| ],"
      "|comment|: |url|,"
      "|structs|:"
      "["
      "  { |name|:|dati|,"
      "    |fields|:"
      "    ["
      "      { |name|:|f1|, |type|:{ |base|:|char|, |dec|:|*| } },"
      "      { |name|:|f2|, |type|:{ |base|:|char|, |dec|:|*| } },"
      "      { |name|:|f3|, |type|:{ |base|:|s_as_u64| } },"
      "      { |name|:|f4|, |c_name|:|c_f4|, |type|:{ |base|:|s_as_i64| } },"
      "      { |name|:|f5 asdfasdf |, |c_name|:|c_f5|, |type|:{ |base|:|char|,  |U|:|convert| } },"
      "      { |name|:|f6|, |type|:{ |base|:|char|, |dec|:|*| }, |loc|:|form| },"
      "      { |name|:|f7|, |type|:{ |base|:|bool|}, |loc|:|query| },"
      "    ]"
      "  }"
      "]"
    "}";

  spec =
    "{ |disabled|:false,\n"
      "|namespace|: [ |A|, |B| ],\n"
      "|comment|: |url|,\n"
      "|defs|:\n"
      "[\n"
#if 1
      "  {\n"
      "    |namespace|:[|C|],\n"
      "    |struct|:|dati|,\n"
      "    |fields|:\n"
      "    [\n"
      "      { |name|:|f1_str|, |type|:{ |base|:|char|, |dec|:|*| }, |comment|:| this is field f1 | },\n"
      "      { |name|:|f2_str|, |type|:{ |base|:|char|, |dec|:|*| } },\n"
      "      { |name|:|f3_int|, |type|:{ |base|:|int|, |c_base|:|code| } },\n"
      "      { |name|:|fx_todo|, |todo|:true, |type|:{ |base|:|int|, |c_base|:|code| } },\n"
      "      { |name|:|f4_uint64|, |c_name|:|c_f4|, |type|:{ |base|:|s_as_u64| } },\n"
      "      { |name|:|f5 asdfasdf |, |c_name|:|c_f5|, |type|:{ |base|:|char|, |dec|:|*| } },\n"
      "      { |name|:|f6_carray|, |type|:{ |base|:|char|, |dec|:|[10]| } },\n"
      "      { |name|:|f7_ntl|, |type|:{ |base|:|int|, |dec|:|ntl| } },\n"
      "      { |name|:|f8_ntl|, |type|:{ |base|:|emoji::dati|, |dec|:|ntl| } },\n"
      "      { |name|:|f9_todo|, |todo|:true, |type|:{ |base|:|emoji::dati|, |dec|:|ntl| } },\n"
      "      { |name|:|f10|, |todo|:false, |type|:{ |base|:|emoji::dati|, |dec|:|*| } },\n"
      "      { |name|:|f11|, |type|:{ |base|:|char|, |dec|:|*|, |converter|:|iso8601| } },\n"
      "      { |name|:|f12|, |type|:{ |base|:|char|, |dec|:|*|}, |inject_if_not|:null },\n"
      "      { |name|:|f13|, |type|:{ |base|:|char|, |dec|:|[12]|}, |inject_if_not|:|| },\n"
      "      { |name|:|f14|, |type|:{ |base|:|char|, |dec|:|*|, |converter|:|iso8601|}, |inject_if_not|:10 },\n"
      "    ]\n"
      "  }\n"
#endif
      " ,{"
      "    |namespace|:[|C|],"
      "    |enum|:|enum1|,"
      "    |items|:"
      "    ["
      "      { |name|:|e1|, |value|:1 },"
      "      { |name|:|e2| },"
      "      { |name|:|e4|, |value|:100 },"
      "      { |name|:|e5| },"
      "      { |name|:|e6| },"
      "    ]"
      "  }"
      "]"
      "}";

  if (argc == 2) {
    s = orka_load_whole_file(argv[1], &len);
  }
  else {
    json_asprintf(&s, spec);
    len = strlen(s);
  }

  /*
  fprintf(stderr, "%.*s\n", len, s);
   */

  struct jc_definition d;
  memset(&d, 0, sizeof(d));
  spec_buffer.start = s;
  spec_buffer.size = len;
  definition_from_json(s, len, &d);
  print_definition(stderr, &d);
  gen_definition(stderr, 0, &d);
  return 0;
}
