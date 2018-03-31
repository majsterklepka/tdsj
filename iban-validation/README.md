# What folder include?

This is a template of futured application. I'm testing possibilities now.

## Main problem?

Calling "activate" signal I getting error. 

        static void activate(GApplication *app, gpointer user_data)
        {

                GObject *window;
                GtkBuilder *builder;

                builder = gtk_builder_new_from_file("interface.glade");
                gtk_builder_connect_signals(builder, app);

                window = gtk_builder_get_object(builder, "window");

                gtk_application_add_window(GTK_APPLICATION(app), GTK_WINDOW(window));
                gtk_widget_show_all(GTK_WIDGET(window));
                g_object_unref(builder);
        }

Function above working perfectly. I would get a typedef struct which will be consisted of..(see below):

        typedef struct app_ {
                GtkBuilder *builder;
                GSList *objects;
        }App;

but with 

        GtkApplication *

it doesn't works.
 
