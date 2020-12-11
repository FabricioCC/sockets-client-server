#include <stdio.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

//para obter os ip
#include <arpa/inet.h>

#define SERVER_PORT 9999

void *tratarCliente(void *param)
{
    int clifd = (int)param; //pega como paramentro o fd do cliente

    int termino = 0;

    //mensagem inicial
    char msg[20] = "\nBEM VINDO! \n";

    char buf[11];
    
    //envia o bem vindo para o cliente
    write(clifd, msg, strlen(msg));

    while (!termino){

        int ehPalindromo = 1;


        bzero(buf, 10);
        //le a mensagem enviada pelo cliente
        read(clifd, buf, 10);

        //verifica se o cliente realmente mandou algo
        if (strlen(buf) > 0){
            printf("Recebi do cliente %d: %s", clifd, buf);
            
            //verifica se os caracteres de trás pra frente são os mesmos de frente pra trás
            for(int i = 0; i < strlen(buf)-1; i++){
                if(buf[i] != buf[strlen(buf)-i-2]){
                    ehPalindromo = 0;
                }
            }

            
            if(ehPalindromo == 1){
                strcpy(msg, "PALINDROMO=TRUE");
                
            }else{
                strcpy(msg, "PALINDROMO=FALSE");
            }

            
            

        }
        //escreve para o cliente se a palvra é palindromo ou nao
        write(clifd, msg, strlen(msg));
        
        //se a mensagem for shutdown, ele dá um exit no servidor e encerra sua execucao
        if (strstr(buf, "SHUTDOWN") != NULL)
        {
            exit(0);
        }
        //se a mensagem for BYEBYE, ele apenas encerra a execucao da thread que enviou
        if (strstr(buf, "BYEBYE") != NULL)
        {

            close();
            pthread_exit(NULL);
        }
            
    }
    
    close(clifd); //para a conexao com todos os clientes
    
}

int main(void)
{
    int serverSocket, clientSocket;

    struct sockaddr_in serverAddr, clientAddr;

    int sizecliaddr;

    //cria o socket para o servidor
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    //array para guardar as threads
    pthread_t clientThread[10];

    //verifica se o socket foi criado corretamente
    if (serverSocket == -1)
    {
        printf("ERRO: criação do socket não teve sucesso\n");
        exit(0);
    }

    //zera os campos da struct da variavel serverAddr (sockaddr_in)
    bzero(&serverAddr, sizeof(serverAddr));

    //preenche os campos da struct, passando a familia, o endereco e a porta
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVER_PORT);


    if (bind(serverSocket, ((struct sockaddr *)&serverAddr), sizeof(serverAddr)) != 0)
    {
        printf("ERROR: server socket bind sem sucesso\n");
        exit(0);
    }

    //verifica se o servidor esta aguardando corretamente os clientes
    if (listen(serverSocket, 4) != 0)
    {
        printf("ERROR: server socket listen sem sucesso\n");
        exit(0);
    }

    //indica em qual porta o socket do servidor está
    printf("server socket listening on port: %d\n", SERVER_PORT);

    clientSocket = 0;

    int qtClientes = 0;

    //limita a quantidade de clientes a 6
    while (!(qtClientes < 0))
    {
        clientSocket = accept(serverSocket, ((struct sockaddr *)&clientAddr), &sizecliaddr);

        //inet_ntoa para mostrar o IP
        printf("Novo cliente conectado - IP: %s!\n", inet_ntoa(clientAddr.sin_addr) );

        //cria uma thread pra o cliente
        pthread_create(&clientThread[qtClientes], NULL, tratarCliente, (void *)clientSocket);


        qtClientes++;
    }

    //aguarda os clientes desconectarem
    for(int i = 0; i < qtClientes; i++){
        pthread_join(clientThread[i], NULL);
    }
}