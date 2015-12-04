#include <config.h>
#include <glib/gi18n.h>
#include <json-glib/json-glib.h>

#include "screenshot-config.h"

#include <stdio.h>
#include <curl/curl.h>

static gint find_chat_id(gchar *bot_tocken, gchar *acc_name);

static void read_telegram_config() {
	GSettings *settings = g_settings_new ("org.gnome.gnome-screenshot");
	gchar *bot_tocken = g_settings_get_string(settings, "bot-tocken");
	gchar *acc_name = g_settings_get_string(settings, "acc-name");
	gint chat_id = g_settings_get_int(settings, "chat-id");
	if (chat_id || (chat_id = find_chat_id(bot_tocken, acc_name))) {
		g_printerr("chat_id:");
	} else {
		// g_printerr("can't find chat_id");
	}
	// g_settings_set_int(settings, "chat-id", )
}


#define BUFF_LEN 512

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	gchar *buf = (gchar *)userp;

	if (realsize > BUFF_LEN * sizeof(gchar)) {
		fprintf(stderr, "not enough memory");
		return 0;
	}

	memcpy(buf, contents, realsize);
	// printf("%s\n", buf);

	return realsize;
}

static gint find_chat_id(gchar *bot_tocken, gchar *acc_name) {
	CURL *curl;
	CURLcode res;

	gchar response[BUFF_LEN];
	curl = curl_easy_init();

	if (curl) {
		gchar url[256];
		sprintf(url, "https://api.telegram.org/%s/getUpdates", bot_tocken);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POST, 0);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);
		res = curl_easy_perform(curl);

		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			        curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);

	}
}



static void send_photo(int chat_id) {

}

int send_telegram() {
	read_telegram_config();
	return 0;
}