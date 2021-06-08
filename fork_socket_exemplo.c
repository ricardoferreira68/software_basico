#include <stdio.h> 
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
void dostuff(int); /* prótitopo  da  função  que  manipula conexões */
void error(char *msg)
{
    perror(msg);
    exit(1);
}
int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen, pid;
    struct sockaddr_in serv_addr, cli_addr;
    if (argc < 2)
    {
        fprintf(stderr, "ERRO, nenhuma porta provida\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERRO ao abrir socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");
        pid = fork();
        if (pid < 0)
            error("ERROR on fork");
        if (pid == 0)
        {
            close(sockfd);
            dostuff(newsockfd);
            exit(0);
        }
        else
            close(newsockfd);
    } /* fim do while */
    return 0;
}
/******** DOSTUFF() *********************
Existirá uma instância separada desta função para cada conexão. 
Esta função manipula toda a comunicação uma vez que uma conexão foi estabelecida.
*****************************************/
void dostuff(int sock)
{
    int n;
    char buffer[256];
    bzero(buffer, 256);
    n = read(sock, buffer, 255);
    if (n < 0)
        error("ERRO lendo do socket");
    printf("Aqui esta a mensagem: %s\n", buffer);
    n = write(sock, "Mensagem recebida", 18);
    if (n < 0)
        error("ERRO escrevendo para o socket");
}
