/*
 * =====================================================================================
 *
 *       Filename:  client_gui.c
 *
 *    Description:  Implemention of GUI chat client.
 *
 *        Version:  1.0
 *        Created:  2010年12月25日 01时36分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <pthread.h>
#include    <gtk/gtk.h>
#include    "protocol.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  process_thread
 *  Description:  Process messages.
 * =====================================================================================
 */
    static int
process_thread ( void *data )
{
    Chat_msg *msg;
    gchar buf[MAX_TEXT_LEN + 20];
    GtkTextBuffer *text_buffer = GTK_TEXT_BUFFER(data);
    GtkTextIter text_iter;
    while (1) {
        msg = chat_msg_pop();
        gtk_text_buffer_get_end_iter(text_buffer, &text_iter);
        switch ( msg->type ) {
            case CHAT_MSG_CHAT:                 /* Chat message */
                sprintf(buf, "%s\n", msg->text);
                gtk_text_buffer_insert(text_buffer, &text_iter, buf, -1);
                break;
            case CHAT_MSG_LIST:                 /* List users */
                sprintf(buf, "Online Users: %s\n", msg->text);
                gtk_text_buffer_insert_with_tags_by_name(text_buffer,
                        &text_iter, buf, -1, "system", NULL);
                break;
        }               /* -----  end switch  ----- */
        chat_msg_destroy(msg);
    }
    return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  on_entry_activate
 *  Description:  Being called when enter in pressed in the entry.
 * =====================================================================================
 */
    static gboolean
on_entry_activate ( GtkWidget *entry, gpointer data )
{
    int client_fd = GPOINTER_TO_INT(data);
    const gchar *text;

    Chat_msg *msg = chat_msg_new();
    text = gtk_entry_get_text(GTK_ENTRY(entry));
    if (strcmp(text, "/list") == 0) {   /* List users */
        msg->type = CHAT_MSG_LIST;
    } else {                            /* Chat message */
        msg->type = CHAT_MSG_CHAT;
        strcpy(msg->text, text);
    }
    msg->fd = client_fd;
    chat_msg_push(msg);
    chat_msg_destroy(msg);

    gtk_entry_set_text(GTK_ENTRY(entry), "");   /* Clear the entry */
    return FALSE;
}       /* -----  end of static function on_entry_activate  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  on_delete_event
 *  Description:  On window close button clicked.
 * =====================================================================================
 */
    static gboolean
on_delete_event ( GtkWidget *widget, GdkEvent *event, gpointer data )
{
    int client_fd = GPOINTER_TO_INT(data);
    /*-----------------------------------------------------------------------------
     *  Send logout message
     *-----------------------------------------------------------------------------*/
    chat_client_logout(client_fd);
    /*-----------------------------------------------------------------------------
     *  ChatSys finalization.
     *-----------------------------------------------------------------------------*/
    if (chat_protocol_exit(client_fd) < 0) {
        exit(1);
    }
    gtk_main_quit();
    return FALSE;
}       /* -----  end of static function on_delete_event  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  show_welcome_message
 *  Description:  Show welcome message in the @text_buffer.
 * =====================================================================================
 */
    static void
show_welcome_message ( GtkTextBuffer *text_buffer )
{
    GtkTextIter text_iter;
    gtk_text_buffer_get_end_iter(text_buffer, &text_iter);
    gtk_text_buffer_insert_with_tags_by_name(text_buffer, &text_iter, 
            "Welcome to ChatSys 0.1!\nType '/list' for online users.\n",
            -1, "system", NULL);
    return ;
}       /* -----  end of static function show_welcome_message  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  main function
 * =====================================================================================
 */
    int
main ( int argc, char *argv[] )
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *text_view;
    GtkWidget *entry;
    GtkTextBuffer *text_buffer;

    int client_fd;
    pthread_t tid;

    if (argc != 3) {
        printf("Usage: chat_client_cli hostname username.\n");
        exit(1);
    }
    /*-----------------------------------------------------------------------------
     *  ChatSys initialization.
     *-----------------------------------------------------------------------------*/
    client_fd = chat_protocol_init(CHAT_CLIENT, 6666, argv[1]);
    if (client_fd < 0) {
        exit(1);
    }
    /*-----------------------------------------------------------------------------
     *  Build GUI
     *-----------------------------------------------------------------------------*/
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(window, 400, 300);
    g_signal_connect(G_OBJECT(window), "delete-event",
            G_CALLBACK(on_delete_event), GINT_TO_POINTER(client_fd));

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_create_tag(text_buffer, "system",
            "weight", PANGO_WEIGHT_BOLD,
            "foreground", "blue", NULL);
    gtk_text_buffer_create_tag(text_buffer, "username",
            "foreground", "red", NULL);
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 5);

    entry = gtk_entry_new();
    g_signal_connect(G_OBJECT(entry), "activate",
            G_CALLBACK(on_entry_activate), GINT_TO_POINTER(client_fd));
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 5);

    gtk_widget_show_all(window);
    gtk_widget_grab_focus(entry);

    /*-----------------------------------------------------------------------------
     *  Send login message to server.
     *-----------------------------------------------------------------------------*/
    chat_client_login(client_fd, argv[2]);
    show_welcome_message(text_buffer);
    /*-----------------------------------------------------------------------------
     *  Create processing thread.
     *-----------------------------------------------------------------------------*/
    pthread_create(&tid, NULL, (void *) process_thread, (void *) text_buffer);

    gtk_main();
    return EXIT_SUCCESS;
}       /* ----------  end of function main  ---------- */

