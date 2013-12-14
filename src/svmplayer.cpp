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
#include "sopcast.h"
//
#include <stdio.h>

#include <iostream>
#include <pthread.h>
//
#include "svmwindow.h"

namespace {

int replay_cnt = 50;
int replay_sleep = 1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct VlcData {
    std::string source;
    Window winid;
};

void stillplay() {
    pthread_mutex_lock ( &mutex );
}

void* vlc_play ( void* data ) {
    libvlc_instance_t* inst;
    libvlc_media_player_t* mp;
    libvlc_media_t* media;
    VlcData& vlcdata = *static_cast<VlcData*> ( data );

    inst = libvlc_new ( 0, NULL );
    media = libvlc_media_new_location ( inst, vlcdata.source.c_str() );
    mp = libvlc_media_player_new_from_media ( media );
    libvlc_media_release ( media );
    libvlc_media_player_set_xwindow ( mp, vlcdata.winid );
    libvlc_media_player_play ( mp );

    // try to reconnect to localhost since sp-sc-auth isn't run immediately
    for ( ; !libvlc_media_player_is_playing ( mp ) && replay_cnt ; --replay_cnt ) {

        libvlc_media_player_stop ( mp );
        libvlc_media_player_play ( mp );

        sleep ( replay_sleep );
    }

    stillplay();

    libvlc_media_player_stop ( mp );
    libvlc_media_player_release ( mp );
    libvlc_release ( inst );
}

}

using namespace svmp;

SVMPlayer::SVMPlayer()
    : m_soport ( 8908 ) {

}

SVMPlayer::~SVMPlayer() {

}

void SVMPlayer::play ( const std::string& url ) {
    if ( m_sopcast.start ( url, m_soport ) != SopCast::SVMSuccess ) {
        finish();
    }

    VlcData data;
    SVMWindow window;
    data.winid = window.create();
    data.source = "http://localhost:" + std::to_string ( m_soport );

    pthread_mutex_lock ( &mutex );

    pthread_t t1;
    pthread_create ( &t1, NULL, &vlc_play, &data );

    window.raise();

    pthread_mutex_unlock ( &mutex );

    pthread_join ( t1, 0 );

    pthread_mutex_unlock ( &mutex );
    pthread_mutex_destroy ( &mutex );

    finish();
}

void SVMPlayer::finish() {
    m_sopcast.stop();
}




