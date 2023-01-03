#include <gtk/gtk.h>

enum
{
	ADDON_PRIORITY,
	ADDON_NAME,
	ADDON_AUTHOR,
	ADDON_CATEGORY,
	ADDON_ENABLED
};

static void
activate (GtkApplication *app, gpointer usrdat)
{
	GtkListStore *model;
	GtkTreeIter iter;
	GtkWidget    *view;
	GtkWidget    *window;
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "List window");
	gtk_window_set_default_size (GTK_WINDOW (window), 300, 700);

	model = gtk_list_store_new (5,
	                            G_TYPE_UINT,
	                            G_TYPE_STRING,
	                            G_TYPE_STRING,
	                            G_TYPE_STRING,
	                            G_TYPE_BOOLEAN);
	gtk_list_store_append (model, &iter);
	gtk_list_store_set (model, &iter,
	                    ADDON_PRIORITY, 2,
	                    ADDON_NAME, "Mod name",
	                    ADDON_AUTHOR, "Mod author",
	                    ADDON_CATEGORY, "Weapons",
	                    ADDON_ENABLED, TRUE,
	                    -1);
	gtk_widget_show (window);
}

int
main (int argc, char** argv)
{
	GtkApplication *app;
	int ret;
	app = gtk_application_new ("demo.main", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	// g_signal_connect ( app context, signal name, pointer to function, ??)
	ret = g_application_run (G_APPLICATION (app), argc, argv);
	// this replaces gtk_init
	// why wasn't there the g_application conversion in g_signal_connect?
	g_object_unref (app);
	return ret;
}