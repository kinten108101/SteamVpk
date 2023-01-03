#include <adwaita.h>
#include "../smkl/io.h"
#include "../smkl/mem.h"
#include "../smkl/type.h"
#include "gio/gmenu.h"
#include "gio/gmenumodel.h"
#include "gtk/gtkcssprovider.h"

#define MV_APPNAME "com.github.kinten108101.matesvpk"

enum
{
    COLUMN_POSITION,
    COLUMN_ID,
    COLUMN_ENABLED,
    COLUMN_COMMENT,
    N_COLUMN
};

int
model_add_entry (GtkTreeStore *model, GtkTreeIter *p, 
                 int pos, uint64_t id, int en, char *comment)
{
    gtk_tree_store_append (model, p, NULL);
    gtk_tree_store_set (model, p,
                        COLUMN_POSITION, pos++,
                        COLUMN_ID, id,
                        COLUMN_ENABLED, en,
                        COLUMN_COMMENT, comment,
                        -1);
    return pos;
}

void
populate_model (GtkTreeStore *model)
{
    GtkTreeIter tree_iter;
    int pos;

    pos = 1;
    pos = model_add_entry (model, &tree_iter, pos, 2889007037, TRUE, "inss francis");
    pos = model_add_entry (model, &tree_iter, pos, 2889005304, TRUE, "inss louis");
    pos = model_add_entry (model, &tree_iter, pos, 2858122798, FALSE, "animal33 mw ellis");
    /*
    csv_parse_file (&csv_charstream, path);
    csv_for_each_line ()
    {
        entry_iter = ??;
        pos = model_add_entry (model, &tree_iter, pos, entry_iter.id, entry_iter.en, entry_iter.comment);
    }
    */
    
}

void
layout_view (GtkTreeView *view)
{
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes ("#", renderer, "text", COLUMN_POSITION, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (view), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes ("ID", renderer, "text", COLUMN_ID, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (view), column);

    renderer = gtk_cell_renderer_toggle_new();
    column = gtk_tree_view_column_new_with_attributes ("Enabled", renderer, "active", COLUMN_ENABLED, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (view), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes ("Comment", renderer, "text", COLUMN_COMMENT, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (view), column);
}

void
app_setup_addonlist (GtkBuilder *builder)
{
    GtkTreeStore *addonlist_store = gtk_tree_store_new (N_COLUMN,
        G_TYPE_UINT64,
        G_TYPE_UINT64,
        G_TYPE_BOOLEAN,
        G_TYPE_STRING);
    populate_model (addonlist_store);

    GtkTreeView *addonlist_view = GTK_TREE_VIEW (gtk_builder_get_object (builder, "addonlist_view") );
    gtk_tree_view_set_model(addonlist_view, GTK_TREE_MODEL (addonlist_store) );
    layout_view (addonlist_view);
}

void
app_setup_menu (GtkBuilder *builder)
{
    GMenu *menu_main = g_menu_new ();

    GMenu *section_dev = g_menu_new ();
    g_menu_append (section_dev, "Link", NULL);
    // GMenuItem *item_link = g_menu_item_new("Link", NULL);
    g_menu_append (section_dev, "Unlink", NULL);

    // approach 1
    //GMenuItem *item_to_section_dev = g_menu_item_new (NULL, NULL);
    //g_menu_item_set_link (item_to_section_dev, G_MENU_LINK_SECTION, G_MENU_MODEL (section_dev));
    //g_menu_append_item(menu_main, item_to_section_dev);

    // approach 2
    // GMenuItem *item_to_section_dev = g_menu_item_new_section(NULL, G_MENU_MODEL (section_dev));
    // g_menu_append_item(menu_main, item_to_section_dev);
    
    // approach 3
    g_menu_append_section(menu_main, NULL, G_MENU_MODEL (section_dev));


    GMenu *section_app = g_menu_new ();
    g_menu_append (section_app, "Preference", NULL);
    g_menu_append (section_app, "About", NULL);
    GMenuItem *item_to_section_app = g_menu_item_new (NULL, NULL);
    g_menu_item_set_link (item_to_section_app, G_MENU_LINK_SECTION, G_MENU_MODEL (section_app));
    g_menu_append_item(menu_main, item_to_section_app);

    GtkPopoverMenu *popover_menu = GTK_POPOVER_MENU (gtk_builder_get_object (builder, "index_popovermenu"));
    if (!popover_menu) mkdie ("Couldn't get popover");
    gtk_popover_menu_set_menu_model( popover_menu, G_MENU_MODEL (menu_main) );
}

void
app_activate (GtkApplication *app,
              gpointer        user_data)
{
    GtkBuilder *builder = gtk_builder_new_from_file ("/home/kinten/Garage/mates-vpk/resources/ui/bay.ui");
    GtkWindow *window = GTK_WINDOW (gtk_builder_get_object(builder, "main_window"));
    gtk_window_set_application( window, app);

    app_setup_addonlist (builder);

    app_setup_menu (builder);

    GtkCssProvider *css_servant = gtk_css_provider_new (); /* alternatively, use gtk_css_provider_load_from_file () */
    gtk_css_provider_load_from_path (css_servant, "/home/kinten/Garage/mates-vpk/resources/css/main.css");
    
    gtk_widget_show ( GTK_WIDGET (window) );
    g_object_unref (builder);
}

int main (int argc, char **argv)
{
    int status = -1;
    
    AdwApplication *app = adw_application_new (MV_APPNAME, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (app_activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    mkwarn ("App is done running!");
    return status;
}