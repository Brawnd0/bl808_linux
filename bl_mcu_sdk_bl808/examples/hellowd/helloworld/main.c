/**
 * @file main.c
 * @brief
 *
 * Copyright (c) 2021 Bouffalolab team
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 */
#include "bflb_platform.h"
#include "hal_uart.h"
#include "hal_mtimer.h"

int main(void)
{
    bflb_platform_init(0);
#ifdef CPU_M0
    MSG("M0 E907 start...\r\n");
    mtimer_init();
    MSG("mtimer clk:%d\r\n", CPU_Get_MTimer_Clock());

    // how do we know psram clk init worked?
    MSG("psram clk init ok!\r\n");
    // MSG("m0 main! size_t:%d\r\n", sizeof(size_t));
    bl_show_info();

    // Disable and invalidate all dcache
    // D0 will still run without this
    csi_dcache_disable();

// D0 will NOT run without this running in m0
#ifdef DUALCORE
    BL_WR_WORD(IPC_SYNC_ADDR1, IPC_SYNC_FLAG);
    BL_WR_WORD(IPC_SYNC_ADDR2, IPC_SYNC_FLAG);
    // Clean cache at IPC_SYNC_ADDR1 0x40000000
    MSG("Dcache clean at %X\r\n", IPC_SYNC_ADDR1 );
    L1C_DCache_Clean_By_Addr(IPC_SYNC_ADDR1, 8);
#endif
#endif

#ifdef CPU_D0

    // #define GLB_AHB_CLOCK_LZ4 (0x0008000000000000UL)

    // GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_LZ4);
    MSG("D0 C906 start...\r\n");
    uint64_t start_time, stop_time;
    mtimer_init();
    MSG("mtimer clk:%d\r\n", CPU_Get_MTimer_Clock());
    // MSG("C906 main! size_t:%d\r\n", sizeof(size_t));
    bflb_platform_delay_ms(100);

    bl_show_info();

    csi_dcache_disable();
#endif


    for (uint8_t i = 0; i < 10; i++) {
#if defined(CPU_M0)
        MSG("hello world from M0!\r\n");
#elif defined(CPU_D0)
        MSG("hello world from D0!\r\n");
#endif
        bflb_platform_delay_ms(200);
    }

    BL_CASE_SUCCESS;
    while (1) {
        bflb_platform_delay_ms(100);
    }
}
