/* Copyright 2014, Gustavo Muro
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

/** \brief This file implements the test of the modbus library
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Modbus CIAA Modbus
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * GMuro        Gustavo Muro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140722 v0.0.1 GMuro implement first tests
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaModbus_slave.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

#define SLAVE_ID  2

/*==================[internal functions declaration]=========================*/
static uint16_t cmd0x03ReadHoldingReg(
       uint16_t start,
       uint16_t quantity,
       uint8_t * exceptioncode,
       uint8_t * buf
       );

/*==================[internal data declaration]==============================*/
static int32_t hModbusSlave;

static const ciaaModbus_slaveCmd_type callbacksStruct =
{
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
};



/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void)
{
   /* ignore calls GetResource */
   GetResource_CMockIgnoreAndReturn(E_OK);

   /* ignore calls ReleaseResource */
   ReleaseResource_CMockIgnoreAndReturn(E_OK);

   /* perform the initialization of modbus slave */
   ciaaModbus_slaveInit();

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void)
{
}

void doNothing(void)
{
}

void test_ciaaModbus_slaveSendMsg(void)
{
   uint8_t pduSend[] = {0x03, 0x00, 0x00, 0x00, 0x01};
   uint8_t pduRecv[50];
   uint8_t id;
   uint32_t size;

   ciaaModbus_slaveSendMsgType(
         hModbusSlave,
         SLAVE_ID,
         pduSend,
         5);

   ciaaModbus_slaveTask(hModbusSlave);

   ciaaModbus_slaveRecvMsg(
         hModbusSlave,
         &id,
         pduRecv,
         size);

   //TEST_ASSERT_EQUAL_UINT8_ARRAY

}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

