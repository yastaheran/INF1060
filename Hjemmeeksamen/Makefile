CC=gcc
CFLAGS=-Wall -Wextra -std=c99

all: client server

run_dollhouse: server
	./server localhost 1060 jobfiles/a_dolls_house.job

run_short_dollhouse: server
	./server localhost 1060 jobfiles/a_dolls_house_short.job
	
run_alice: server
	./server localhost 1060 jobfiles/alice.job
	
run_alice_short: server
	./server localhost 1060 jobfiles/alice_short.job

run_corrupt: server
	./server localhost 1060 jobfiles/corrupt.job

run_huck: server
	./server localhost 1060 jobfiles/huckleberry.job

run_huck_short: server
	./server localhost 1060 jobfiles/huckleberry_short.job

run_meta: server
	./server localhost 1060 jobfiles/metamorphosis.job

run_meta_short: server
	./server localhost 1060 jobfiles/metamorphosis_short.job

run_pap: server
	./server localhost 1060 jobfiles/pride_and_predjudice.job

run_pap_short: server
	./server localhost 1060 jobfiles/pride_and_predjudice_short.job

run_client: client
	./client localhost 1060
	
debug_client: client_d
	./client localhost 1060 d 1

debug_server: server_d
	./server localhost 1060 jobfiles/a_dolls_house_short.job d 1
	
client: client.c
	$(CC) -g $(CFLAGS) $^ -o $@

server: server.c
	$(CC) -g $(CFLAGS) $^ -o $@

clean:
	rm -f server client