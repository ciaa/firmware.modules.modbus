/* Copyright 2014, Mariano Cerdeiro
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief This file implements the Modbus main functionality
 **
 ** This file implements the main functionality of the Modbus
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Modbus CIAA Modbus
 ** @{ */

/*
 * Initials     Name
 * MaCe         Mariano Cerdeiro
 * GMuro        Gustavo Muro
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140623 v0.0.1 initials initial
 */

/*==================[inclusions]=============================================*/
#include "ciaaModbus_config.h"
#include "ciaaModbus_transport.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_stdbool.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

#define TOTAL_TRANSPORTS   (  CIAA_MODBUS_TOTAL_TRANSPORT_ASCII   + \
                              CIAA_MODBUS_TOTAL_TRANSPORT_RTU     + \
                              CIAA_MODBUS_TOTAL_TRANSPORT_TCP )

typedef struct
{
   bool inUse;
   int32_t fdDevice;
   int32_t hModbusLowLayer;
   ciaaModbus_transportMode_enum mode;
}ciaaModbus_transportObj_type;


/*==================[internal data declaration]==============================*/

static ciaaModbus_transportObj_type ciaaModbus_transportObj[TOTAL_TRANSPORTS] =
{
   {
      .fdDevice = -1,
      .hModbusLowLayer = -1,
      .mode = 0,
   },
};

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
extern int32_t ciaaModbus_transportOpen(
      int32_t fildes,
      ciaaModbus_transportMode_enum mode)
{
   int32_t hModbusTransport;

   switch (mode)
   {
      case CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER:
      case CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE:
         hModbusTransport = TOTAL_TRANSPORTS-1;
         break;

      default:
         hModbusTransport = -1;
         break;
   }

   GetResource(MODBUSR);

   while (hModbusTransport >= 0)
   {
      if (ciaaModbus_transportObj[hModbusTransport].inUse == false)
      {
         ciaaModbus_transportObj[hModbusTransport].inUse = true;
         ciaaModbus_transportObj[hModbusTransport].fdDevice = fildes;
         ciaaModbus_transportObj[hModbusTransport].mode = mode;
         break;
      }
      hModbusTransport--;
   }

   ReleaseResource(MODBUSR);

   return hModbusTransport;
}

extern int32_t ciaaModbus_transportRecv(
      int32_t hModTra,
      uint8_t *buf,
      uint32_t *len)
{
   return 0;
}

extern int32_t ciaaModbus_transportSend(
      int32_t hModTra,
      uint8_t *buf,
      uint32_t len)
{
   return 0;
}

/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

