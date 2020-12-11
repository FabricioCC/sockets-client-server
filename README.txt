Primeiramente, é necessário abrir um terminal que executar o servidor
e mais alguns terminais que serão os clientes

para executar o servidor, é necessário compilar o arquivo server.c
para isso, só colocar a seguinte linha de comando:
gcc -pthread server.c -o server

depois de compilar, só executar, com o comando:
./server

após isso o servidor estará pronto, agora falta criar os clientes
para isso, é necessario compilar uma vez o arquivo client.c
usa-se a seguinte linha de comando:
gcc -pthread client.c -o client

e depois, em cada terminal cliente, executar o arquivo compilado,
usando a seguinte linha de comando:
./client

Assim, que executar o cliente, só mandar mensagens pra o servidor com as palavras
que ele retornará se a palavra é palindromo ou não
para encerrar o cliente, basta digitar "BYEBYE"
para encerrar todo o servidor, só digitar "SHUTDOWN", alguns clientes ainda
tentarão mandar mensagens mas só apertar ENTER até eles pararem definitavemente.