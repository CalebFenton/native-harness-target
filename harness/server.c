#include "server.h"

void start_server(int port, JavaVM *vm, JNIEnv *env, decryptString_t decryptString, jobject cryptor_instance) {
  int sockfd, newsockfd;
  socklen_t clilen;
  char *buffer;
  struct sockaddr_in serv_addr, cli_addr;
  int pid;
  int ret;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int option = 1;
  // Set socket to be reusable because we use fork and socket might get stuck
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, & option, sizeof(option));
  if (sockfd < 0) {
    perror(" [!] ERROR opening socket");
    return;
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror(" [!] ERROR binding socket");
    return;
  }

  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  while (1) {
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
    if (newsockfd < 0) {
      perror(" [!] ERROR on socket accept");
      return;
    }

    pid = fork();
    if (pid < 0) {
      perror(" [!] ERROR on socket fork");
      return;
    }

    jstring enc_str = NULL;
    if (pid == 0) {
      // Child process
      close(sockfd);

      printf(" [+] Connection accepted\n");
      do {
        buffer = (char *) malloc(sizeof(char) * (MAX_LINE_LEN + 1));
        ret = read_line(newsockfd, buffer, MAX_LINE_LEN);
        if (ret < 0) {
          break;
        }

        if (ret == 0) {
          free(buffer);
          break;
        }

        enc_str = (*env)->NewStringUTF(env, buffer);
        jstring result = decryptString(env, cryptor_instance, enc_str);
        const char *str = (*env)->GetStringUTFChars(env, result, NULL);
        printf(" [+] Sending decrypted string\n");
        write(newsockfd, str, strlen(str));

        free(buffer);
      } while (1);

      exit(0);
    } else {
      // Parent
      close(newsockfd);
    }
  }
}

static ssize_t read_line(int fd, void *buffer, size_t n) {
  ssize_t numRead;
  size_t totRead;
  char *buf;
  char ch;

  if (n <= 0 || buffer == NULL) {
    errno = EINVAL;
    return -1;
  }

  buf = buffer;

  totRead = 0;
  for (;;) {
    numRead = read(fd, &ch, 1);

    if (numRead == -1) {
      if (errno == EINTR) {
        continue;
      } else {
        return -1;
      }
    } else if (numRead == 0) {
      if (totRead == 0) {
        return 0;
      } else {
        break;
      }
    } else {
      // Don't include newline char in buffer
      if (ch == '\n') {
        break;
      }

      if (totRead < n - 1) {
        totRead++; *buf++ = ch;
      }
    }
  }

  *buf = '\0';
  return totRead;
}