#include <gtk/gtk.h>

#define MV_APPNAME "com.github.kinten108101.matesvpk"

void
print_hello (GtkWidget *widget, gpointer data)
{
	g_print ("Hello world!\n");
}

enum
{
	COLUMN_POSITION,
	COLUMN_ENABLED,
	COLUMN_NAME,
	COLUMN_TAG,
	N_COLUMN
};

int
model_add_entry (GtkTreeStore *model, GtkTreeIter *p, 
	             int pos, int en, char *name, char *categories)
{
	gtk_tree_store_append (model, p, NULL);
	gtk_tree_store_set (model, p,
                        COLUMN_POSITION, pos++,
                        COLUMN_ENABLED, en,
                        COLUMN_NAME, name,
                        COLUMN_TAG, categories,
                        -1);
	return pos;
}

void
populate_model (GtkTreeStore *model)
{
	GtkTreeIter p;
	int pos;

	pos = 1;
	pos = model_add_entry(model, &p, pos, FALSE, "a new addon", "Survivors, Nick");
	pos = model_add_entry(model, &p, pos, TRUE, "another new addon", "Survivors, Ellis");
}

void
layout_view (GtkWidget *tree_view)
{
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes ("Position", renderer, "text", COLUMN_POSITION, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), column);

	renderer = gtk_cell_renderer_toggle_new();
	column = gtk_tree_view_column_new_with_attributes ("Enabled", renderer, "active", COLUMN_ENABLED, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes ("Name", renderer, "text", COLUMN_NAME, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes ("Tags", renderer, "text", COLUMN_TAG, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), column);
}

GtkWidget *
mv_setup_list ()
{
	GtkWidget *tree_view;
	GtkTreeStore *tree_model;
	
	tree_model = gtk_tree_store_new (N_COLUMN, G_TYPE_UINT64, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING);
	/* pos | en | name | tags */
	populate_model (tree_model);
	
	tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (tree_model));
	layout_view (tree_view);
	
	return tree_view;
}

void
mv_activate (GtkApplication *app, gpointer data)
{
	GtkWidget *window = gtk_application_window_new (app);
	gtk_window_set_title ( GTK_WINDOW (window), "hi");
	gtk_window_set_default_size ( GTK_WINDOW (window), 200, 400);
	
	GtkWidget *box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_halign (box, GTK_ALIGN_CENTER);
	gtk_widget_set_valign (box, GTK_ALIGN_START);

	gtk_window_set_child ( GTK_WINDOW (window), box);
	GtkWidget *list = mv_setup_list ();
	GtkWidget *btn_makelink = gtk_button_new_with_label ("Make Links");
	GtkWidget *btn_removelink = gtk_button_new_with_label ("Remove Links");
	GtkWidget *btn_quit = gtk_button_new_with_label ("Quit");
	gtk_box_append ( GTK_BOX (box), list);
	gtk_box_append ( GTK_BOX (box), btn_makelink);
	gtk_box_append ( GTK_BOX (box), btn_removelink);
	gtk_box_append ( GTK_BOX (box), btn_quit);
	g_signal_connect (btn_makelink, "clicked", G_CALLBACK (print_hello), NULL);
	g_signal_connect_swapped (btn_quit, "clicked", G_CALLBACK (gtk_window_destroy), window);


	gtk_widget_show (window); /* where is the typecasting to GTK_WINDOW ? */
}

void
mv_shutdown ()
{
	g_print ("Shutting down with grace!\n");
}

int main (int argc, char *argv[])
{
	/* create app instance */
	GtkApplication *app = gtk_application_new (MV_APPNAME, G_APPLICATION_DEFAULT_FLAGS);
	/* attach signal handlers */
	g_signal_connect (app, "activate", G_CALLBACK (mv_activate), NULL); /* quirky macro for additional type casting - a GObject moment */
	g_signal_connect (app, "shutdown", G_CALLBACK (mv_shutdown), NULL);
	/* run app */
	int app_status = g_application_run (G_APPLICATION (app), argc, argv);
	/* post-shutdown cleanup */
	g_object_unref (app);
	return app_status;
}