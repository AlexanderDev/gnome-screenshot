#include <config.h>
#include <glib/gi18n.h>
#include <json-glib/json-glib.h>

#include "screenshot-config.h"

#include <stdio.h>
#include <curl/curl.h>

static gint find_chat_id(gchar *bot_tocken, gchar *acc_name);


#define BUFF_LEN 512
gchar buff[BUFF_LEN];

struct chat_data {
	gint chat_id;
	gchar *acc_name;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct chat_data *cd = (struct chat_data*)userp;

	JsonParser *parser;
	JsonNode *root;
	parser = json_parser_new ();
	json_parser_load_from_data (parser, (gchar *)contents, realsize, NULL);
	root = json_parser_get_root (parser);
	JsonObject *chat_object = json_object_get_object_member(
	                              json_object_get_object_member(
	                                  json_array_get_object_element(
	                                      json_object_get_array_member(json_node_get_object(root), "result"),
	                                      0), "message"), "chat");
	gchar *last_name = json_object_get_string_member(chat_object , "last_name");
	gchar *first_name = json_object_get_string_member(chat_object , "first_name");

	sprintf(buff, "%s %s", first_name, last_name);
	if (!g_strcmp0(buff, cd->acc_name)) {
		cd->chat_id = json_object_get_int_member(chat_object, "id");
	}
	printf("%s\n", buff);
	g_object_unref (parser);

	return realsize;
}


static gint find_chat_id(gchar *bot_tocken, gchar *acc_name) {
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	struct chat_data data = {0, acc_name};

	if (curl) {
		gchar url[256];
		sprintf(url, "https://api.telegram.org/%s/getUpdates", bot_tocken);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POST, 0);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);
		res = curl_easy_perform(curl);

		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			        curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
		return data.chat_id;
	}
}



struct TConfig
{
	GSettings *settings;
	gchar *bot_tocken;
	gchar *acc_name;
	gint chat_id;
};

struct TConfig config;

static void read_telegram_config() {
	config.settings = g_settings_new ("org.gnome.gnome-screenshot");
	config.bot_tocken = g_settings_get_string(config.settings, "bot-tocken");
	config.acc_name = g_settings_get_string(config.settings, "acc-name");
	config.chat_id = g_settings_get_int(config.settings, "chat-id");
	gint new_chat_id;
	if (config.chat_id || (new_chat_id = find_chat_id(config.bot_tocken, config.acc_name))) {
		if (!config.chat_id) {
			g_settings_set_int(config.settings, "chat-id", new_chat_id);
			config.chat_id = new_chat_id;
		}
	}
}

static void send_photo() {

}

int send_telegram() {
	read_telegram_config();
	send_photo();
	return 0;
}