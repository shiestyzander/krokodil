#include <kk_dashboard.h>
#include <kk_css.h>
#include <kk_client.h>
#include <stdbool.h>

#include "../config/token.h"

struct attack s_attack;

struct attack s_attack = {
    false,  // mass_channel_enabled
    "NULL", // mass_channel_name
    0,      // mass_channel_count
    true,   // mass_channel_type
    false,  // guild_name_enabled
    "NULL"  // guild_name[20]
};
/**
 * Callbacks
 */

/**
 * Module 1 (Callbacks)
 * Mass Channel
 */

void mass_channel_enable_switched(GtkSwitch *widget,
                                  gboolean state,
                                  gpointer user_data)
{
    if (state == TRUE)
    {
        s_attack.mass_channel_enabled = true;
    }
    else
    {
        s_attack.mass_channel_enabled = false;
    }
    printf("\n\n\n%d", s_attack.mass_channel_enabled);
}

void mass_channel_entry_active(GtkWidget *widget, gpointer data)
{
    s_attack.mass_channel_name = (char *)gtk_entry_get_text(GTK_ENTRY(widget));
}

void mass_channel_count_change(GtkWidget *self,
                               GtkScrollType *scroll,
                               gpointer user_data)
{
    s_attack.mass_channel_count = (int )gtk_spin_button_get_value_as_int(self);
}

void *client_init(char *TOKEN);

void start_button_pressed(GtkWidget *widget, gpointer data)
{
}

void activateDashboard(GtkApplication *app,
                       gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *logs_box;
    GtkWidget *button_start;
    GtkWidget *button_stop;
    GtkWidget *button_logs;

    // Mass channel widgets
    GtkWidget *mass_channel_box;
    GtkWidget *mass_channel_enable_switch;
    GtkWidget *mass_channel_label;
    GtkWidget *mass_channel_name_entry;
    GtkWidget *mass_channel_count_spin;
    GtkWidget *mass_channel_type_combobox;

    // Guild name widgets
    GtkWidget *guild_name_box;
    GtkWidget *guild_name_enable_switch;
    GtkWidget *guild_name_label;
    GtkWidget *guild_name_entry;

    GtkAdjustment *adj;

    gtk_css();

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Krokodil");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 500);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_set_halign(box, GTK_ALIGN_START);
    gtk_widget_set_valign(box, GTK_ALIGN_START);

    /**
     * Module 1
     * Mass Channel
     */

    mass_channel_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(box), mass_channel_box, TRUE, TRUE, 0);
    gtk_widget_set_name(mass_channel_box, "mass_channel_box");
    g_object_set(mass_channel_box, "margin-top", 10, NULL);
    g_object_set(mass_channel_box, "margin-left", 10, NULL);
    gtk_widget_set_halign(mass_channel_box, GTK_ALIGN_START);
    gtk_widget_set_valign(mass_channel_box, GTK_ALIGN_START);

    mass_channel_enable_switch = gtk_switch_new();
    gtk_switch_set_state(mass_channel_enable_switch, false);
    gtk_container_add(GTK_CONTAINER(mass_channel_box), mass_channel_enable_switch);
    g_object_set(mass_channel_enable_switch, "margin-left", 10, NULL);
    g_object_set(mass_channel_enable_switch, "margin-right", 10, NULL);
    g_signal_connect(mass_channel_enable_switch, "state-set", G_CALLBACK(mass_channel_enable_switched), NULL);

    mass_channel_label = gtk_label_new("MASS CHANNEL");
    gtk_container_add(GTK_CONTAINER(mass_channel_box), mass_channel_label);
    gtk_widget_set_halign(mass_channel_label, GTK_ALIGN_CENTER);
    gtk_widget_set_name(mass_channel_label, "mass_channel_label");
    g_object_set(mass_channel_label, "margin-left", 10, NULL);
    g_object_set(mass_channel_label, "margin-right", 10, NULL);

    mass_channel_name_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(mass_channel_name_entry), 15);
    gtk_container_add(GTK_CONTAINER(mass_channel_box), mass_channel_name_entry);
    g_object_set(mass_channel_name_entry, "margin-left", 10, NULL);
    g_object_set(mass_channel_name_entry, "margin-right", 10, NULL);
    gtk_entry_set_text(mass_channel_name_entry, "channel-name");
    g_signal_connect(GTK_EDITABLE(mass_channel_name_entry), "changed", G_CALLBACK(mass_channel_entry_active), NULL);

    adj = (GtkAdjustment *)gtk_adjustment_new(1.0, 1.0, 200.0, 1.0,
                                              5.0, 0.0);
    mass_channel_count_spin = gtk_spin_button_new(adj, 0, 0);
    gtk_container_add(GTK_CONTAINER(mass_channel_box), mass_channel_count_spin);
    g_object_set(mass_channel_count_spin, "margin-left", 10, NULL);
    g_object_set(mass_channel_count_spin, "margin-right", 10, NULL);
    g_signal_connect(mass_channel_count_spin, "change-value", G_CALLBACK(mass_channel_count_change), NULL);

    mass_channel_type_combobox = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(mass_channel_type_combobox), NULL, "Text");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(mass_channel_type_combobox), NULL, "Voice");
    gtk_combo_box_set_active(GTK_COMBO_BOX(mass_channel_type_combobox), 0);
    g_object_set(mass_channel_type_combobox, "margin-left", 10, NULL);
    g_object_set(mass_channel_type_combobox, "margin-right", 10, NULL);
    gtk_container_add(GTK_CONTAINER(mass_channel_box), mass_channel_type_combobox);

    /**
     * Module 2
     * Guild Name
     */

    guild_name_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(box), guild_name_box, TRUE, TRUE, 0);
    gtk_widget_set_name(guild_name_box, "guild_name_box");
    g_object_set(guild_name_box, "margin-top", 10, NULL);
    g_object_set(guild_name_box, "margin-left", 10, NULL);
    gtk_widget_set_halign(guild_name_box, GTK_ALIGN_START);
    gtk_widget_set_valign(guild_name_box, GTK_ALIGN_START);

    guild_name_enable_switch = gtk_switch_new();
    gtk_switch_set_state(guild_name_enable_switch, false);
    gtk_container_add(GTK_CONTAINER(guild_name_box), guild_name_enable_switch);
    g_object_set(guild_name_enable_switch, "margin-left", 10, NULL);
    g_object_set(guild_name_enable_switch, "margin-right", 10, NULL);

    guild_name_label = gtk_label_new("GUILD NAME");
    gtk_container_add(GTK_CONTAINER(guild_name_box), guild_name_label);
    gtk_widget_set_name(guild_name_box, "guild_name_label");
    gtk_widget_set_halign(guild_name_label, GTK_ALIGN_CENTER);
    gtk_widget_set_name(mass_channel_label, "guild_name_label");
    g_object_set(guild_name_label, "margin-left", 20, NULL);
    g_object_set(guild_name_label, "margin-right", 20, NULL);

    guild_name_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(guild_name_entry), 20);
    gtk_container_add(GTK_CONTAINER(guild_name_box), guild_name_entry);
    g_object_set(guild_name_entry, "margin-left", 10, NULL);
    g_object_set(guild_name_entry, "margin-right", 10, NULL);
    gtk_entry_set_text(guild_name_entry, "Nuked by Krokodil");

    button_start = gtk_button_new_with_label("START");
    gtk_container_add(GTK_CONTAINER(box), button_start);
    g_signal_connect(button_start, "clicked", G_CALLBACK(start_button_pressed), mass_channel_enable_switch);
    g_signal_connect_swapped(button_start, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    //  gtk_widget_set_halign(button_start, GTK_ALIGN_END);
    //  gtk_widget_set_valign(button_start, GTK_ALIGN_END);
    gtk_widget_set_name(button_start, "button_start");

    gtk_widget_show_all(window);
}

void *startDashboard()
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.krokodil.dashboard", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activateDashboard), NULL);
    status = g_application_run(G_APPLICATION(app), NULL, NULL);
    g_object_unref(app);
}