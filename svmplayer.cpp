/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2013  <copyright holder> <email>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "svmplayer.h"
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>

namespace {

Display* _d;
Window _xid;
libvlc_instance_t* _inst;
libvlc_media_player_t* _mp;
libvlc_media_t* _m;
std::string source;
int terminate = 0;

void* vlc_play ( void* ) {
    /* Load the VLC engine */
    _inst = libvlc_new ( 0, NULL );

    /* Create a new item */
    _m = libvlc_media_new_location ( _inst, source.c_str() );

    /* Create a media player playing environement */
    _mp = libvlc_media_player_new_from_media ( _m );

    /* No need to keep the media now */
    libvlc_media_release ( _m );

    libvlc_media_player_set_xwindow ( _mp, _xid );

    /* play the media_player */
    libvlc_media_player_play ( _mp );

    while ( !terminate ) {
        // play
    }

    /* Stop playing */
    libvlc_media_player_stop ( _mp );

    /* Free the media_player */
    libvlc_media_player_release ( _mp );

    libvlc_release ( _inst );
}

}

using namespace svmp;

SVMPlayer::SVMPlayer() {

}

SVMPlayer::~SVMPlayer() {

}

void SVMPlayer::play ( const std::string& url ) {
    source = url;
    _sp.start ( url );

    int s;
    XEvent e;

    /* open connection with the server */
    _d = XOpenDisplay ( NULL );
    if ( _d == NULL ) {
        printf ( "Cannot open display\n" );
        exit ( 1 );
    }
    s = DefaultScreen ( _d );

    /* create window */
    _xid = XCreateSimpleWindow ( _d, RootWindow ( _d, s ), 10, 10, 500, 400, 1,
                                 BlackPixel ( _d, s ), WhitePixel ( _d, s ) );

    // Process Window Close Event through event handler so XNextEvent does Not fail
    Atom delWindow = XInternAtom ( _d, "WM_DELETE_WINDOW", 0 );
    XSetWMProtocols ( _d , _xid, &delWindow, 1 );

    /* select kind of events we are interested in */
    XSelectInput ( _d, _xid, ExposureMask | KeyPressMask );

    /* map (show) the window */
    XMapWindow ( _d, _xid );

    pthread_t t1;
    pthread_create ( &t1, NULL, &vlc_play, NULL );

    /* event loop */
    while ( 1 ) {
        XNextEvent ( _d, &e );
        /* draw or redraw the window */
        if ( e.type==Expose ) {
            XFillRectangle ( _d, _xid, DefaultGC ( _d, s ), 0, 0, 500, 400 );
        }
//         /* exit on key press */
//         if ( e.type==KeyPress )
//             break;

        // Handle Windows Close Event
        if ( e.type==ClientMessage )
            break;
    }

    terminate = 1;

    pthread_join ( t1, 0 );

    XDestroyWindow ( _d, _xid );
    XCloseDisplay ( _d );

}

