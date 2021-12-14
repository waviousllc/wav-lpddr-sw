/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Standard includes. */
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/* System includes. */
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>

/* Kernel includes. */
#include <kernel/printf.h>

/* WDDR includes. */
#include <wddr/interface.h>

/* Firmware includes. */
#include <firmware/phy_task.h>

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
#define TASK_PRI    (configMAX_PRIORITIES - 1)
#define TASK_PORT   (7000)

enum
{
    PHY_CMD_TYPE_EVENT = 0x1,
    PHY_CMD_TYPE_IND,
};

enum
{
    PHY_IND_POST_SWITCH = 0x1,
    PHY_IND_PENDING_SWITCH,
    PHY_IND_PHYMSTR_ENTER,
    PHY_IND_PHYMSTR_EXIT,
    PHY_IND_PHYUPD_ENTER,
    PHY_IND_PHYUPD_EXIT,
    PHY_IND_CTRLUPD_DONE,
};

struct phy_command_hdr
{
    uint8_t type;
    uint8_t len;
};

/*******************************************************************************
**                           VARIABLE DECLARATIONS
*******************************************************************************/
// TODO: What's needed?
static struct wddr_device_t
{
    int client_fd;
    uint8_t id;
} wddr_instance;

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
static void xPhyServerTask( void *pvParameters );
TaskHandle_t task_handle = NULL;
static void send_phy_indication(wddr_handle_t wddr, uint8_t indication);

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/

wddr_handle_t wddr_init(uint32_t base)
{
    if (task_handle != NULL)
    {
        return NULL;
    }

    xTaskCreate( xPhyServerTask, "PHY Server Task", configMINIMAL_STACK_SIZE, &wddr_instance, TASK_PRI, &task_handle );

    wddr_instance.client_fd = -1;
    return &wddr_instance;
}

wddr_return_t wddr_boot(wddr_handle_t wddr, wddr_boot_cfg_t cfg)
{
    configPRINTF(("%ld: WDDR: boot called\n", xTaskGetTickCount()));
    return WDDR_SUCCESS;
}

wddr_return_t wddr_prep_switch(wddr_handle_t wddr,
                               uint8_t freq_id,
                               wddr_msr_t msr)
{
    configPRINTF(("%ld: WDDR: prep called\n", xTaskGetTickCount()));
    return WDDR_SUCCESS;
}

wddr_return_t wddr_sw_freq_switch(wddr_handle_t wddr,
                                  uint8_t freq_id,
                                  wddr_msr_t msr)
{
    configPRINTF(("%ld: WDDR: SW freq switch called\n", xTaskGetTickCount()));
    return WDDR_SUCCESS;
}

void wddr_handle_post_switch(wddr_handle_t wddr)
{
    configPRINTF(("%ld: WDDR: Handle post switch called\n", xTaskGetTickCount()));
    send_phy_indication(wddr, PHY_IND_POST_SWITCH);
}

void wddr_handle_pending_switch(wddr_handle_t wddr)
{
    configPRINTF(("%ld: WDDR: Handle pending switch called\n", xTaskGetTickCount()));
    send_phy_indication(wddr, PHY_IND_PENDING_SWITCH);
}

uint8_t wddr_get_current_msr(wddr_handle_t wddr)
{
    // Intentionally Empty
    configPRINTF(("%ld: WDDR: Get Current MSR called\n", xTaskGetTickCount()));
    return 0;
}

uint8_t wddr_get_next_msr(wddr_handle_t wddr)
{
    configPRINTF(("%ld: WDDR: Get Next MSR called\n", xTaskGetTickCount()));
    return 0;
}

void wddr_phymstr_enter(wddr_handle_t wddr, dfi_phymstr_req_t *req)
{
    configPRINTF(("%ld: WDDR: PHYMSTR enter called\n", xTaskGetTickCount()));
    send_phy_indication(wddr, PHY_IND_PHYMSTR_ENTER);
}

void wddr_phymstr_exit(wddr_handle_t wddr)
{
    configPRINTF(("%ld: WDDR: PHYMSTR exit called\n", xTaskGetTickCount()));
    send_phy_indication(wddr, PHY_IND_PHYMSTR_EXIT);
}

void wddr_phyupd_enter(wddr_handle_t wddr, dfi_phyupd_type_t type)
{
    configPRINTF(("%ld: WDDR: PHYUPD enter called\n", xTaskGetTickCount()));
    send_phy_indication(wddr, PHY_IND_PHYUPD_ENTER);
}

void wddr_phyupd_exit(wddr_handle_t wddr)
{
    // Intentionally Empty
    configPRINTF(("%ld: WDDR: PHYUPD exit called\n", xTaskGetTickCount()));
    send_phy_indication(wddr, PHY_IND_PHYUPD_EXIT);
}

void wddr_ctrlupd_done(wddr_handle_t wddr)
{
    configPRINTF(("%ld: WDDR: CTRLUPD done called\n", xTaskGetTickCount()));
}

void wddr_disable_ctrlupd_interface(wddr_handle_t wddr)
{
    configPRINTF(("%ld: WDDR: Disable CTRLUPD interface called\n", xTaskGetTickCount()));

}

void wddr_iocal_update_phy(wddr_handle_t wddr)
{
    //configPRINTF(("%ld: WDDR: IOCAL Update called\n", xTaskGetTickCount()));
}

wddr_return_t wddr_iocal_calibrate(wddr_handle_t wddr)
{
    //configPRINTF(("%ld: WDDR: IOCAL calibrate called\n", xTaskGetTickCount()));
    return WDDR_SUCCESS;
}

void wddr_gate_external_interfaces(wddr_handle_t wddr, bool should_gate)
{
    configPRINTF(("%ld: WDDR: Gate external interfaces called\n", xTaskGetTickCount()));

}

void wddr_enable_loopback(wddr_handle_t wddr)
{
    configPRINTF(("%ld: WDDR: Enable Loopback called\n", xTaskGetTickCount()));
}

wddr_return_t wddr_load_packets(wddr_handle_t wddr, List_t *packets)
{
    configPRINTF(("%ld: WDDR: Load packets called\n", xTaskGetTickCount()));
    return WDDR_SUCCESS;
}

wddr_return_t wddr_send_packets(wddr_handle_t wddr)
{
    configPRINTF(("%ld: WDDR: send packets called\n", xTaskGetTickCount()));
    return WDDR_SUCCESS;
}

wddr_return_t wddr_unload_packets(wddr_handle_t wddr, List_t *packets)
{
    configPRINTF(("%ld: WDDR: Unload packets called\n", xTaskGetTickCount()));
    return WDDR_SUCCESS;
}

bool wddr_validate_recv_data(wddr_handle_t wddr,
                             const command_data_t *data,
                             const command_data_sig_t *sig,
                             burst_length_t bl,
                             uint8_t dq_byte_mask)
{
    configPRINTF(("%ld: WDDR: validate receive data called\n", xTaskGetTickCount()));
    return true;
}

static bool is_valid_errno( void )
{
    switch(errno)
    {
        case EINTR:
        case EAGAIN:
            return true;
        default:
            return false;
    }
}

static size_t recv_data(wddr_handle_t wddr, void *buff, size_t len)
{
    // Read data from the client
    int recv_len = recv(wddr->client_fd, buff, len, MSG_WAITALL);

    // Client closed connection
    if (recv_len == 0)
    {
        configPRINTF(("closing client.\n"));
        close(wddr->client_fd);
        wddr->client_fd = -1;
    }

    else if (recv_len == -1)
    {
        if (is_valid_errno())
        {
            return 0;
        }
        exit(1);
    }

    return recv_len;
}

static void xPhyServerTask( void *pvParameters )
{
    wddr_handle_t wddr = (wddr_handle_t) pvParameters;
    const TickType_t xDelay = pdMS_TO_TICKS(1); // 1 ms
    int listener;
    struct sockaddr_in addr = {0};
    struct sockaddr_in client_addr = {0};
    struct phy_command_hdr hdr;

    configPRINTF(("xPhyServerTask: started...\n"));

    // Create a socket to listen for incoming messages
    if ((listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        configPRINTF(("xPhyServerTask: Couldn't open socket.\n"));
        exit(1);
    }

    // Make non-blocking so that thread can sleep via FreeRTOS
    fcntl(listener, F_SETFL, O_NONBLOCK);

    // Open socket on port at localhost address for listening
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TASK_PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(listener, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1)
    {
        configPRINTF(("xPhyServerTask: Couldn't bind socket.\n"));
        exit(1);
    }

    if (listen(listener, 1) == -1)
    {
        configPRINTF(("xPhyServerTask: listen failed.\n"));
        exit(1);
    }

    while (1)
    {
        // Sleep Task each time through the loop
        vTaskDelay(xDelay);

        // Wait for incoming connection
        if (wddr->client_fd == -1)
        {
            socklen_t addr_len = sizeof(struct sockaddr_in);
            wddr->client_fd = accept(listener, (struct sockaddr *)&client_addr, &addr_len);

            // Some errors are okay since using non-blocking socket
            if (wddr->client_fd == -1)
            {
                if (is_valid_errno())
                {
                    continue;
                }
                configPRINTF(("xPhyServerTask: accept failed.\n"));
                exit(1);
            }

            configPRINTF(("xPhyServerTask: client connected.\n"));
        }
        // Client is connected
        else
        {
            // Read header
            int recv_len = recv_data(wddr, &hdr, sizeof(struct phy_command_hdr));

            if (recv_len == 0 || hdr.len == 0)
            {
                continue;
            }

            uint8_t buff[hdr.len];
            recv_len = recv_data(wddr, buff, hdr.len);
            if (recv_len == 0)
            {
                configPRINTF(("Warning: Received incomplete PHY command\n"));
            }

            // Create firmware event
            fw_msg_t msg = {.event = buff[0], .data = NULL};
            fw_phy_task_notify(&msg);
        }
    }
}

static void send_phy_indication(wddr_handle_t wddr, uint8_t indication)
{
    struct phy_command_hdr ind_hdr = {
        .type = PHY_CMD_TYPE_IND,
        .len = 1,
    };

    taskENTER_CRITICAL();
    {
        if (wddr->client_fd >= 0)
        {
            // Header
            send(wddr->client_fd, &ind_hdr, sizeof(struct phy_command_hdr), 0);

            // Payload
            send(wddr->client_fd, &indication, sizeof(uint8_t), 0);
        }
    }
    taskEXIT_CRITICAL();
}
