/* Nguyen Thanh Nam - 18020925 */
/* Y tuong: Gui thong tin header cho server, roi gui du lieu 
            Doc thong tin header tu server, kiem tra do co phai la file khong
            Neu la file, ghi du lieu ra file moi, neu khong, viet tin nhan ra man hinh */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERV_PORT 9000
#define BUFF_SIZE 5120

int main(int argc, char* argv[]) {
    int sock_fd;
    char server_addr[] = "127.0.0.1";
    struct sockaddr_in serv_addr;
    char recv_buf[BUFF_SIZE], sent_buf[BUFF_SIZE];

    uint16_t msg_len, is_file, recv_data_len;
    uint32_t file_len;

    // create socket file descriptor 
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket");
        exit(1);
    } else {
        printf("Socket created.\n");
    }

    // create socket 
    // set value before connect 
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr =  inet_addr(server_addr);

    // connect 
    if (connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connect");
        exit(1);
    } else {
        printf("Connected to server\n");
    }

    // write and read 
    while (1) {
        printf("Client: ");
        scanf(" %[^\n]", sent_buf);
        msg_len = htons(strlen(sent_buf)); // convert msg header to network byte order 
        if (strcmp(sent_buf, "QUIT") == 0) {
            write(sock_fd, &msg_len, sizeof(uint16_t)); // send msg header 
            write(sock_fd, sent_buf, strlen(sent_buf));// send msg 
            break;
        }

        // check sending 
        write(sock_fd, &msg_len, sizeof(uint16_t));
        int sentDataLen = write(sock_fd, sent_buf, strlen(sent_buf));
        if (sentDataLen < 0) {
            printf("Error when sending");
            break;
        }

        // read msg from server
        read(sock_fd, &is_file, sizeof(uint16_t));
        is_file = ntohs(is_file); 
        if (is_file == 1) {
            // GET file
            FILE *fp = fopen("cpy", "w");
            read(sock_fd, &file_len, sizeof(uint32_t)); // read header-> get file size 
            int remain_bytes = ntohl(file_len); 
            while (remain_bytes > 0) {
                if (remain_bytes >= BUFF_SIZE) {
                    read(sock_fd, recv_buf, BUFF_SIZE); // read from socket to recv_buf
                    fwrite(recv_buf, 1, BUFF_SIZE, fp); // write from recv_buf to file 
                    remain_bytes -= BUFF_SIZE;
                } else {
                    read(sock_fd, recv_buf, remain_bytes);
                    fwrite(recv_buf, 1, remain_bytes, fp);
                    remain_bytes = 0;
                }
            }
            printf("Server: Enter file name: \n");
            fclose(fp);
        } else if (is_file == 0) {
            // normal msg
            read(sock_fd, &msg_len, sizeof(uint16_t)); // read header
            msg_len = ntohs(msg_len);
            recv_data_len = read(sock_fd, recv_buf, msg_len); // read msg 
            recv_buf[recv_data_len] = '\0'; 
            printf("Server: %s\n", recv_buf);

            if (strcmp(recv_buf, "File not found") == 0) {
                // if file not exists, server will auto close after sent msg 
                // need to close client manually 
                close(sock_fd);
                exit(1);
            }
        } else {
            // read is_file get weird number
            perror("Weird is_file number, closing...");
            exit(1);
        }
    }
    bzero(recv_buf, BUFF_SIZE);
    bzero(sent_buf, BUFF_SIZE);
}