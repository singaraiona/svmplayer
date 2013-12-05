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
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

namespace {
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

SopCast::SopCast() {

}

SopCast::~SopCast() {

}

int SopCast::start ( const std::string& url, int port ) {

    pid_t pid = 0;
    std::string launch = url + " " + std::to_string ( _soport ) + " " + std::to_string ( port );

    pid = fork();

    if ( pid < 0 ) {
        //fork error
        perror ( "fork" );
        exit ( EXIT_FAILURE );
    }
    if ( pid == 0 ) {
        std::string path = getpath();
        std::string target = "LD_LIBRARY_PATH=" + path + "lib " + path + "sp-sc-auth";
        std::cout << target << " " << launch << std::endl;
        execl ( target.c_str(), launch.c_str() , 0, 0 );
        exit ( 0 );
    }

    return 0;
}
