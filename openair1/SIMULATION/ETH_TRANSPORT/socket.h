/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file socket.h
* \brief
* \author Lionel Gauthier
* \date 2011
* \version 1.0
* \company Eurecom
* \email: lionel.gauthier@eurecom.fr
*/

#ifndef __SOCKET_H__
#    define __SOCKET_H__
#    ifdef SOCKET_C
#        define private_socket(x) x
#        define public_socket(x) x
#    else
#        define private_socket(x)
#        define public_socket(x) extern x
#    endif
#    include "stdint.h"
public_socket (void socket_setnonblocking (int sockP);
              )
public_socket (int make_socket_inet (int typeP, uint16_t * portP, struct sockaddr_in *ptr_addressP);
              )
#endif
