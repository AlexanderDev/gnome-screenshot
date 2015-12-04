#include <config.h>
#include <glib/gi18n.h>

#include "screenshot-config.h"


void read_telegram_config(){
	GSettings *settings=g_settings_new ("org.gnome.gnome-screenshot");
	gchar *bot_tocken = g_settings_get_string(settings, "bot-tocken");
	gchar *acc_name = g_settings_get_string(settings, "acc-name");
	gint chat_id = g_settings_get_int(settings, "chat-id");
	// g_settings_set_int(settings, "chat-id", )  
	printf("bot-tocken:%s\n", bot_tocken);
}


int send_telegram(){
	read_telegram_config();
	return 0;
}

void send_photo(int chat_id){

}