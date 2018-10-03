#include <string.h>

#include "../include/global.h"
#include "../include/control_header_lib.h"
#include "../include/network_util.h"
#include "../include/connection_manager.h"


void crash(int sock_index){
        char *cntrl_response_header, *cntrl_response;
        uint16_t payload_len, response_len;
        payload_len = 0;
        cntrl_response_header = create_response_header(sock_index, 4, 0, 0);

        response_len = CNTRL_RESP_HEADER_SIZE;
        cntrl_response = (char *) malloc(response_len);

        /* Copy Header */
        memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
        free(cntrl_response_header);

        sendALL(sock_index, cntrl_response, response_len);

        free(cntrl_response);

        exit(0);

        return;
}
