#include <stdio.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

//define a porta do servidor
#define SERVER_PORT 9999

int main(void)
{
    int clientSocket;
    int terminou = 0;

    struct sockaddr_in serverAddr;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    //verifica se o socket foi criado corretamente
    if (clientSocket == -1)
    {
        printf("ERROR: client socket creation error");
        exit(0);
    }

    //zera os campos da variavel serverAddr (struct sockaddr_in)
    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(SERVER_PORT);

    bind(clientSocket, ((struct sockaddr_in *)&serverAddr), sizeof(serverAddr));

    int conexao = connect(clientSocket, (struct sockaddr_in *)&serverAddr, sizeof(serverAddr));

    //verifica se conectou corretamente ao servidor
    if (conexao != 0)
    {
        printf("ERROR: Connection error!\n");
        exit(0);
    }

    printf("Cliente conectado com sucesso a porta: %d\n", SERVER_PORT);

    //vai guardar a mensagem recebida do servidor
    char mensagemRecebida[21];
    //vai guardar a mensagem que sera enviada
    char mensagemEnviada[11];

    while (!terminou)
    {
        //zera os campos da string
        bzero(mensagemRecebida, 20);
        bzero(mensagemEnviada, 10);

        //le a string enviada pelo servidor
        read(clientSocket, mensagemRecebida, 20);

        //verifica se realmente recebeu algo
        if (strlen(mensagemRecebida) > 0)
            printf("Recebi do servidor: %s\n", mensagemRecebida);


        printf("Digite o que deseja enviar ao servidor: ");
        fgets(mensagemEnviada, 10, stdin);

        sleep(1);

        printf("Vou enviar: %s\n", mensagemEnviada);

        //envia a mensagem para o servidor
        write(clientSocket, mensagemEnviada, strlen(mensagemEnviada) + 1);

        //se for alguma dessas mensagens, encerra a execucao do cliente que a mandou
        if (strstr(mensagemEnviada, "BYEBYE") != NULL || strstr(mensagemEnviada, "SHUTDOWN")!=NULL)
        {
            terminou = 1;
        }

        
    }

    printf("Até a próxima\n");
}