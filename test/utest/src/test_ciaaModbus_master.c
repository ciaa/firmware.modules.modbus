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
/** \addtogroup Modbus CIAA Modbus master test
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * GMuro        Gustavo Muro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20141129 v0.0.1 GMuro implement first tests
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaModbus_master.h"
#include "mock_os.h"

/*==================[macros and definitions]=================================*/

#define CIAA_MODBUS_STARTING_ADDRESS      0X1234
#define CIAA_MODBUS_QUANTITY_OF_HOLD_REG  0x0002
#define SLAVE_ID                          2

/*==================[internal functions declaration]=========================*/

/*==================[internal data declaration]==============================*/
static uint8_t callBackData_slaveId;
static uint8_t callBackData_numFunc;
static uint8_t callBackData_exceptioncode;

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void modbusMaster_cbEndOfComm(
      uint8_t slaveId,
      uint8_t numFunc,
      uint8_t exceptioncode)
{
   callBackData_slaveId = slaveId;
   callBackData_numFunc = numFunc;
   callBackData_exceptioncode = exceptioncode;
}


/*==================[external functions definition]==========================*/

/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void)
{
   /* ignore calls GetResource */
   GetResource_IgnoreAndReturn(E_OK);

   /* ignore calls ReleaseResource */
   ReleaseResource_IgnoreAndReturn(E_OK);

   GetTaskID_IgnoreAndReturn(E_OK);

   /* init module modbus master */
   ciaaModbus_masterInit();
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

/** \brief test function Open
 **
 ** this function test modbus master open
 **
 **/
void test_ciaaModbus_masterOpen_01(void)
{
   uint32_t loopi;
   int32_t hModbusMaster[CIAA_MODBUS_TOTAL_MASTERS+1];

   for (loopi = 0 ; loopi < (CIAA_MODBUS_TOTAL_MASTERS+1) ; loopi++)
   {
      /* open modbus master */
      hModbusMaster[loopi] = ciaaModbus_masterOpen();
   }

   for (loopi = 0 ; loopi < CIAA_MODBUS_TOTAL_MASTERS ; loopi++)
   {
      /* verify */
      TEST_ASSERT_NOT_EQUAL(hModbusMaster[loopi],-1);
   }

   TEST_ASSERT_EQUAL(hModbusMaster[loopi],-1);
}

/** \brief test function read holding register
 **
 ** this function test modbus master read holding registers
 ** with callback
 **
 **/
void test_ciaaModbus_masterCmd0x03ReadHoldingReg_01(void)
{
   int32_t hModbusMaster;
   uint16_t startAddress;
   uint16_t quantity;
   int16_t hrValue[CIAA_MODBUS_QUANTITY_OF_HOLD_REG];
   uint8_t slaveIdRecv;
   uint8_t pduRecv[256];
   uint8_t pduRecvExp[256] =
   {
      0X03,
      (CIAA_MODBUS_STARTING_ADDRESS >> 8) & 0XFF,
      (CIAA_MODBUS_STARTING_ADDRESS >> 0) & 0XFF,
      (CIAA_MODBUS_QUANTITY_OF_HOLD_REG >> 8) & 0XFF,
      (CIAA_MODBUS_QUANTITY_OF_HOLD_REG >> 0) & 0XFF,
   };
   uint8_t pduSend[256] =
   {
      0X03,
      CIAA_MODBUS_QUANTITY_OF_HOLD_REG * 2,
      0XAA,
      0X55,
      0X11,
      0X22,
   };

   uint32_t sizeRecv;
   uint32_t sizeRecvExp = 5;

   /* open modbus master */
   hModbusMaster = ciaaModbus_masterOpen();

   /* request read holding register */
   ciaaModbus_masterCmd0x03ReadHoldingReg(
         hModbusMaster,
         CIAA_MODBUS_STARTING_ADDRESS,
         CIAA_MODBUS_QUANTITY_OF_HOLD_REG,
         hrValue,
         SLAVE_ID,
         modbusMaster_cbEndOfComm);

   /* perform task modbus master */
   ciaaModbus_masterTask(hModbusMaster);

   /* receive pdu from master */
   ciaaModbus_masterRecvMsg(
         hModbusMaster,
         &slaveIdRecv,
         pduRecv,
         &sizeRecv);

   /* send message to master */
   ciaaModbus_masterSendMsg(
         hModbusMaster,
         SLAVE_ID,
         pduSend,
         6);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, slaveIdRecv);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(pduRecvExp, pduRecv, 5);

   TEST_ASSERT_EQUAL_UINT32(sizeRecvExp, sizeRecv);

   TEST_ASSERT_EQUAL_UINT16(0XAA55, hrValue[0]);

   TEST_ASSERT_EQUAL_UINT16(0X1122, hrValue[1]);

   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, callBackData_slaveId);

   TEST_ASSERT_EQUAL_UINT8(0x03, callBackData_numFunc);

   TEST_ASSERT_EQUAL_UINT8(0x00, callBackData_exceptioncode);
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

