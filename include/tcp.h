/******************************************************************************
 * This file is part of The Ekans Webserver.                                  *
 * Copyright (C) 2011 Samuel C. Payson                                        *
 *                                                                            *
 * Ekans is free software: you can redistribute it and/or modify it under the *
 * terms of the GNU General Public License as published by the Free Software  *
 * Foundation, either version 3 of the License, or (at your option) any later *
 * version.                                                                   *
 *                                                                            *
 * Ekans is distributed in the hope that it will be useful, but WITHOUT ANY   *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS  *
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more     *
 * details.                                                                   *
 *                                                                            *
 * You should have received a copy of the GNU General Public License along    *
 * with Ekans. If not, see <http://www.gnu.org/licenses/>.                    *
 ******************************************************************************/

#ifndef _EKANS_TCP_H_
#define _EKANS_TCP_H_

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

typedef int SOCK;

SOCK get_listener(const char * port);

#endif /* tcp.h */
