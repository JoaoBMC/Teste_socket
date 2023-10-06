all: server client run_client run_server cls
server:
		gcc -c ../Servidor/server.c -o server.o -lpthread
		gcc -o smain ../Servidor/main.c server.o -lpthread
client:
		gcc -c ../Cliente/client.c -o client.o -lpthread
		gcc -o cmain ../Cliente/main.c client.o -lpthread

srun: server
		./smain
crun: client
		./cmain
clean:
		rm -rf *.o