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
    SleepTimeSec = 1
};

char* homedir = getenv ( "HOME" );
char execpath[1024];
char ld_library_path[1040];

}

using namespace svmp;

SopCast::SopCast()
    :m_soport ( 3908 ) {

    snprintf ( execpath, sizeof ( execpath ), homedir, "/sopcast" );
    snprintf ( ld_library_path, sizeof ( ld_library_path ), "LD_LIBRARY_PATH=", execpath );
    snprintf ( execpath, sizeof ( execpath ), "svmplayer" );
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

    m_sopid = fork();

    SVM_ASSERT_MSG ( m_sopid >= 0, "Fork error." );

    if ( m_sopid == 0 ) {
        char* sopenv[] = {ld_library_path, NULL};
        char* sopargs[] = {execpath, curl, csoport, cport, NULL};
        execve ( execpath, sopargs, sopenv );
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

