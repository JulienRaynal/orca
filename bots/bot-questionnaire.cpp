#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <libdiscord.h>
#include "orka-utils.h" // for orka_timestamp_ms()

using namespace discord;

void on_ready(client *client, const user::dati *me)
{
  fprintf(stderr, "\n\nQuestionnaire-Bot succesfully connected to Discord as %s#%s!\n\n",
      me->username, me->discriminator);

  (void)client;
}

void set_role(client *client, const uint64_t guild_id, const uint64_t channel_id, const guild::member::dati *member)
{
  role::dati *role = role::dati_alloc();

  char role_name[128];
  snprintf(role_name, sizeof(role_name), "beginner_%s", member->user->username);
  guild::create_role::params params1 = {
    .name = role_name
  };
  guild::create_role::run(client, guild_id, &params1, role);

  if (role->id) {
    char text[150];
    snprintf(text, sizeof(text), "Succesfully created <@&%" PRIu64 "> role", role->id);

    channel::message::create::params params2 = {
      .content = text
    };
    channel::message::create::run(client, channel_id, &params2, NULL);
  }

  guild::modify_member::params params3 = {
    .nick = "Newbie"
  };
  //ja_u64_list_append(&params3.roles, (struct ja_u64*)&role->id); @todo

  guild::modify_member::run(client, guild_id, member->user->id, &params3, NULL);

  role::dati_free(role);
}

void on_member_join(client *client, const user::dati *me, const uint64_t guild_id, const guild::member::dati *member)
{
  if (member->user->bot) // ignore bots
    return;

  channel::dati *ch = channel::dati_alloc();

  char channel_name[32];
  snprintf(channel_name, sizeof(channel_name), "welcome_%s", member->user->username);
  guild::create_channel::params params1 = {
    .name = channel_name,
    .topic = "Questionnaire."
  };
  guild::create_channel::run(client, guild_id, &params1, ch);

  if (ch->id) { // post message to channel if succesfully created
    set_role(client, guild_id, ch->id, member);

    char text[1024];
    snprintf(text, sizeof(text), "Welcome, <@!%" PRIu64 ">!", member->user->id);
    channel::message::create::params params2 = {
      .content = text
    };
    //channel::overwrite::append
    channel::message::create::run(client, ch->id, &params2, NULL);
  }

  channel::dati_free(ch);
}

int main(int argc, char *argv[])
{
  const char *config_file;
  if (argc > 1)
    config_file = argv[1];
  else
    config_file = "bot.config";

  setlocale(LC_ALL, "");

  global_init();

  client *client = config_init(config_file);
  assert(NULL != client);

  setcb(client, GUILD_MEMBER_ADD, &on_member_join);

  printf("\n\nTHIS IS A WORK IN PROGRESS"
         "\nTYPE ANY KEY TO START BOT\n");
  fgetc(stdin); // wait for input

  run(client);

  cleanup(client);

  global_cleanup();
}


