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

#include "svmwindow.h"
#include <stdlib.h>
#include <unistd.h>
//
#include <svm_assert.h>

using namespace svmp;

SVMWindow::SVMWindow()
    : m_width ( 600 )
    , m_height ( 500 )
    , m_title("SvmPlayer") {

}

SVMWindow::~SVMWindow() {
    XDestroyWindow ( m_display, m_xid );
    XCloseDisplay ( m_display );
}

int SVMWindow::create() {
    m_display = XOpenDisplay ( NULL );

    SVM_ASSERT_MSG ( m_display , "Cannot open display" );

    s = DefaultScreen ( m_display );
    m_xid = XCreateSimpleWindow ( m_display, RootWindow ( m_display, s ), 10, 10, m_width, m_height, 1,
                                  BlackPixel ( m_display, s ), BlackPixel ( m_display, s ) );
    Atom delWindow = XInternAtom ( m_display, "WM_DELETE_WINDOW", 0 );
    XSetWMProtocols ( m_display , m_xid, &delWindow, 1 );
    XSelectInput ( m_display, m_xid, ExposureMask | KeyPressMask );
    XMapWindow ( m_display, m_xid );
    XStoreName(m_display, m_xid, m_title.c_str());
    
    return m_xid;
}

void SVMWindow::raise() {
    XEvent e;
    for ( ;; ) {
        XNextEvent ( m_display, &e );

        if ( e.type == Expose ) {
        } else if ( e.type == ClientMessage ) {
            break;
        } else if ( e.type == DestroyNotify ) {
            break;
        }
    }
}
