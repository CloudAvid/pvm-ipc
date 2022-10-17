/**
 * \file macros.h
 * defines common parameter structures for managing/manipulating them.
 *
 * Copyright 2010,2022 Cloud Avid Co. (www.cloudavid.com)
 * \author Ali Nasrolahi
 *
 * PVM-IPC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PVM-IPC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IPC.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <pparam/macros.h>

#define GNUTLS_ERROR_DESC(err) gnutls_strerror(err)
#define CHECK(x)               assert((x) >= 0)
#define LOOP_CHECK(rval, cmd)                                                                      \
    do {                                                                                           \
        rval = cmd;                                                                                \
    } while (rval == GNUTLS_E_AGAIN || rval == GNUTLS_E_INTERRUPTED)

#define SOCKET_HUGE_BUFFER_SIZE    (1 << 20)
#define SOCKET_DEFAULT_BUFFER_SIZE (1 << 14)
#define SOCKET_SMALL_BUFFER_SIZE   (1 << 10)
