/* Nguyễn Thành Nam - 18020925 */
/* Ý tưởng: Đưa phần code xử lý hành động của server vào một hàm riêng, gọi là behave
            Mỗi khi có server accept một kết nối, tạo một luồng riêng
            Mỗi luồng có thể tải file, gửi tin nhắn riêng cho server  
            Code được dựa trên code của BTH số 5
            Bài thực hành số 5 em nộp muộn, không biết là anh có thể châm chước chấm cho em được không, em chấp nhận bị trừ điểm còn hơn là được 0*/
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h> // exit()
#include <string.h> // bzero
#include <arpa/inet.h> // inet_addr
#include <unistd.h> // read, write 
#include <sys/stat.h> //  struct stat
#include <pthread.h>

#define SERV_PORT 9000
#define BUFF_SIZE 5120

enum state_t  {W_CMD, W_NAME, W_GET};

void* behave(void* args) {
    int cli_fd = *((int*)args);
    free(args); // I wont use this
    
    enum state_t state = W_CMD;
    char recv_buf[BUFF_SIZE], sent_buf[BUFF_SIZE], file_name[30];
    uint16_t msg_len, is_file = htons(0), recv_data_len;
    uint32_t file_len;
    int file_size;

    while (1) {
        read(cli_fd, &msg_len, sizeof(uint16_t));
        msg_len = ntohs(msg_len);
        if ((recv_data_len = read(cli_fd, recv_buf, msg_len)) < 0) {
            perror("read");
            exit(1);
        }
        recv_buf[recv_data_len] = '\0';
        if (strcmp(recv_buf, "QUIT") == 0) {
            printf("Closing connection...\n");
            close(cli_fd);
            break;
        } else if (state == W_CMD) {
            if (strcmp(recv_buf, "GET") == 0) {
                strcpy(sent_buf, "Enter file name: ");
                state = W_NAME;
            } else {
                strcpy(sent_buf, "Invalid command, try 'GET'");
            }
        } else if (state == W_NAME) {
            FILE *fp;
            fp = fopen(recv_buf, "r");
            if (fp == NULL) {
                bzero(sent_buf, BUFF_SIZE);
                strcpy(sent_buf, "File not found");

                // send msg to client before exit
                is_file = htons(0);
                write(cli_fd, &is_file, sizeof(uint16_t));
                msg_len = htons(strlen(sent_buf));
                write(cli_fd, &msg_len, sizeof(uint16_t));
                write(cli_fd, sent_buf, strlen(sent_buf));

                printf("File not found\n");
                close(cli_fd);
                exit(1);
            } else {
                // send file name, file size to client 
                struct stat st;
                strcpy(file_name, recv_buf); // save file name 
                stat(file_name, &st); 
                file_size = st.st_size;
                sprintf(sent_buf, "File name: %s | File size: %d bytes", file_name, file_size); // format output of sent_buf 
                state = W_GET;
            }
            fclose(fp);
        } else if (state == W_GET) {
            if (strcmp(recv_buf, "GET IT") == 0) {
                is_file = htons(1);
            } else {
                strcpy(sent_buf, "Wanna get it? Use 'GET IT'");
            }
        }
        
        if (ntohs(is_file) == 0) {
            // if data is not file 
            is_file = htons(0);
            write(cli_fd, &is_file, sizeof(uint16_t));
            msg_len = htons(strlen(sent_buf));
            write(cli_fd, &msg_len, sizeof(uint16_t));
            write(cli_fd, sent_buf, strlen(sent_buf));

        } else if (ntohs(is_file) == 1) {
            // data is file 
            // already has file size, file name
            FILE *fp = fopen(file_name, "r");
            int remain_bytes = file_size;

            // send header to client first
            write(cli_fd, &is_file, sizeof(uint16_t));
            file_len = htonl(file_size);
            write(cli_fd, &file_len, sizeof(uint32_t));

            // send data 
            while (remain_bytes > 0) {
                int bytes_sent;
                int bytes_read;

                if (remain_bytes >= BUFF_SIZE) {
                    bytes_read = fread(sent_buf, 1, BUFF_SIZE, fp); // read from file and store in sent_buf 
                    // printf("."); -- Why server console not print it normally? 
                } else {
                    bytes_read = fread(sent_buf, 1, remain_bytes, fp);
                    // printf("!"); -- Why server console not print it normally?
                }
                bytes_sent = write(cli_fd, sent_buf, bytes_read);
                if (bytes_sent < 0) {
                    perror("write");
                    exit(1);
                }
                remain_bytes -= bytes_read;
            }
            state = W_NAME;
            is_file = htons(0); // set is_file to false
            fclose(fp);
        }

        bzero(recv_buf, BUFF_SIZE);
        bzero(sent_buf, BUFF_SIZE);
    }    
}

int main(int argc, char* argv[]) {
    int serv_fd, *cli_fd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;

    // create listen socket
    // it's neccessary to wrap in a () before compare to 0
    // otherwise, socket won't be created successfully, we
    // get error at bind() 
    if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("listen socket");
        exit(1);
    } else {
        printf("Listen socket created!\n");
    }

    // bind 
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((bind(serv_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))) < 0) {
        perror("bind");
        exit(1);
    } else {
        printf("Binded!\n");
    }

    // listen
    if (listen(serv_fd, 5) < 0) {
        perror("listen");
        exit(1);
    } else {
        printf("Awaiting connection...\n");
    }

    pthread_t tid;
    // set attribute
    pthread_attr_t at_detatched;
    pthread_attr_init(&at_detatched);
    // to save resource
    pthread_attr_setdetachstate(&at_detatched, PTHREAD_CREATE_DETACHED);

    while (1) {
        cli_len = sizeof(cli_addr);
        cli_fd = malloc(sizeof(int));
        *cli_fd = accept(serv_fd, (struct sockaddr*) &cli_addr, &cli_len);
        printf("Received connection from a client %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
        // read and write 
        pthread_create(&tid, &at_detatched, behave, cli_fd);
    }
}