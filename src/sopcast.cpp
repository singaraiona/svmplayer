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

#include "sopcast.h"
//
#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <string.h>
#include <svm_assert.h>

namespace {

#pragma GCC diagnostic ignored "-Wwrite-strings"

enum {
    SleepTimeSec = 10
};

char* sopenv[] = {"LD_LIBRARY_PATH=/home/volonter/sopcast-gui/", NULL};

std::string getpath() {
    std::string path = "";
    pid_t pid = getpid();
    char buf[20] = {0};
    sprintf ( buf,"%d",pid );
    std::string _link = "/proc/";
    _link.append ( buf );
    _link.append ( "/exe" );
    char proc[512];
    int ch = readlink ( _link.c_str(),proc,512 );
    if ( ch != -1 ) {
        proc[ch] = 0;
        path = proc;
        std::string::size_type t = path.find_last_of ( "/" );
        path = path.substr ( 0,t );
    }

    //return path + "/";

    return "/home/volonter/sopcast-gui/";
}

}

using namespace svmp;

SopCast::SopCast()
    :m_soport ( 3908 ) {

}

SopCast::~SopCast() {

}

int SopCast::start ( const std::string& url, int port ) {
    char curl[256];
    char csoport[16];
    char cport[16];

    strncpy ( curl, url.c_str(), url.size() + 1 );
    snprintf ( csoport, sizeof ( csoport ), "%d", m_soport );
    snprintf ( cport, sizeof ( cport ), "%d", port );

    char* sopargs[] = {"/home/volonter/sopcast-gui/sp-sc-auth", curl, csoport, cport, NULL};

    m_sopid = fork();

    SVM_ASSERT_MSG ( m_sopid >= 0, "Fork error." );

    if ( m_sopid == 0 ) {
        execve ( "/home/volonter/sopcast-gui/sp-sc-auth", sopargs, sopenv );
        exit ( SVMSuccess );
    }

    sleep ( SleepTimeSec );

    return SVMSuccess;
}

void SopCast::stop() {
    if ( m_sopid ) {
        kill ( m_sopid, SIGTERM );
        int status;
        waitpid ( m_sopid, &status, CLD_EXITED | CLD_KILLED );
    }
}

