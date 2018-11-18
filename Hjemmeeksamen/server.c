// Biblioteker
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <signal.h>

// Globale variabler
FILE* fp;
int sock;
int client_sock;

//debugger definisjoner
#define INFO_STRING "INFO"
#define WARN_STRING "WARN"
#define ERR_STRING  " ERR"
#define ERR_LEVEL 3
#define WARN_LEVEL 2
#define INFO_LEVEL 1

//Debugger variabler
int debug_mode = 0;
char debug_buf[128] = {0};
char info_type[3][5] = { ERR_STRING, WARN_STRING, INFO_STRING };

/***
 * Job-struct for jobbene i job-fila.
 ***/
typedef struct job {
	int error;
	int checksum; //fikse
	unsigned char type;
	unsigned int len;
	char txt[255];
} jobb;

/***
* Printer ut hva som blir gjort, hva som skjer og hvilket. For øyeblikket er det kun
* info-level som blir brukt. Kilder brukt for å implementere denne metoden ligger i 
* README.txt.
*
* input:
*		int msg_level: hvilket debuggingsnivå man er på
*		char* print_string: informasjonen som skal printes ut.
***/
void debug_print(int msg_level, char* print_string) {
    if(debug_mode > 0) {
        if(debug_mode >= INFO_LEVEL) {
            printf(">>>%d<<<[%s] %s\n", getpid(), info_type[msg_level-1], print_string);
        } else if(debug_mode >= WARN_LEVEL && msg_level <= WARN_LEVEL) {
            printf(">>>%d<<<[%s] %s\n", getpid(), info_type[msg_level-1], print_string);
        } else if(debug_mode >= ERR_LEVEL && msg_level <= ERR_LEVEL) {
            printf(">>>%d<<<[%s] %s\n", getpid(), info_type[msg_level-1], print_string);
        }
    }
}

/***
 * Metoden printer ut hvordan brukeren skal bruke programmet. Kilder brukt for å 
 * implementere denne metoden ligger i README.txt.
 *
 * input:
 *		int argc: Tar inn antall argumenter.
 *		char* argv[]: Tar inn array med argumentene.
 * return:
 *		int : Returnerer 1 dersom brukeren ikke har sendt inn riktig antall argumenter.
 ***/
int usage(int argc, char* argv[]) {
    if(argc != 4 || strcmp(argv[4], "d")) {
        printf("NORMAL USAGE: %s ip port jobliste-fil\n", argv[0]);
        printf("DEBUG USAGE: %s ip port jobliste-fil d (mode: 0/1) \n", argv[0]);
        return 1;
    }
    return 0;
}

/***
* Metoden tar inn en error melding som så skrives ut til stderr. Kilder brukt for å 
* implementere denne metoden ligger i README.txt.
* 
* input:
* 	char* msg: error meldingen
***/
void error(char* msg) {
	if(debug_mode == 1) {
		sprintf(debug_buf, "Error occured");
		debug_print(INFO_LEVEL, debug_buf);
    }
    perror(msg);
    exit(EXIT_FAILURE);
}

/***
 * Henter én jobb fra job-filen, og returnerer den. Jobben legges inn i en job struct 
 * for enkel sending.
 *
 * return:
 *		struct job: Struct som inneholder jobbdata.
 ***/
jobb get_job() {
	jobb tmp;
	
	tmp.error = 0;
	fread(&tmp.type, sizeof(char), 1, fp);
	
	if(debug_mode ==1) {
		sprintf(debug_buf, "Waiting for connection to %d", tmp.type);
		debug_print(INFO_LEVEL, debug_buf);
    }
	fread(&tmp.len, 1, 1, fp);
	int ret = fread(&tmp.txt, tmp.len, 1, fp);
	
	if (!ret) {
		tmp.error = 1;
	}
	
	//gå tilbake til starten av jobbteksten.
	fseek(fp, -tmp.len, SEEK_CUR);
	
	/*
	* Beregning av checksum
	*/
	int cs = 0;
	int i;
	int len = (int) tmp.len;
	div_t res;
	unsigned char c[len];
	
	for(i = 0; i < len; i++) {
		fread(&c[i], 1, 1, fp);
	}
	
	for(i = 0; i < len; i++) {
	//	printf("%c \t", c[i]);
		cs += c[i];
	//	printf("%d \n", cs);
	}
	
	res = div(cs, 32);
	tmp.checksum = res.quot;
	
	if(debug_mode ==1) {
		sprintf(debug_buf, "Job created");
		debug_print(INFO_LEVEL, debug_buf);
    }
	
	return tmp;
}

/***
 * Avslutter programmet om brukeren trykker ctrl+c (^c). Kilder brukt for å implementere 
 * denne metoden ligger i README.txt.
 *
 * input:
 *     int sig: signal
 ***/
void ctrl_chandler(int sig) {
	signal(sig, SIG_IGN);
	if(debug_mode ==1) {
		sprintf(debug_buf, "Exiting with CTRL + C.");
		debug_print(INFO_LEVEL, debug_buf);
    }
    
	fclose(fp);
	send(client_sock, "q", strlen("q"), 0); 
	close(client_sock);
	close(sock);
	exit(0);
}

/***
 * Metoden tar inn en string og returnerer en array med ip-addressen. Kilder brukt for å
 * implementere denne metoden ligger i README.txt.
 *
 * input:
 * 		char* arg: en ip-adresse som streng
 *		char* ip: array med ip-adressen.
 ***/
void get_ip_addr(char* arg, char* ip) {
    struct hostent* host;
    struct in_addr** addr_list;
	
	// Henter informasjon om hosten
	host = gethostbyname(arg);
    if (host == NULL) {
        error("Error host name not found.");
    }
 
    addr_list = (struct in_addr**)host->h_addr_list;
    for(int i = 0; addr_list[i] != NULL; i++) {
        strcpy(ip, inet_ntoa(*addr_list[i]));
    }
}

/***
 * Metoden oppretter en socket og returnerer den. Kilder brukt for å implementere denne 
 * metoden ligger i README.txt.
 *
 * input:
 *		char* : en char pointer til porten
 * return:
 *		int : socket
 ***/
int create_socket(char ip[], char* port) {
	int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(debug_mode == 1) {	
		sprintf(debug_buf, "Socket created: %d", s);
		debug_print(INFO_LEVEL, debug_buf);
	}
	
	int pnr = atoi(port);
	if(debug_mode == 1) {	
		sprintf(debug_buf, "IP-address in use: %d", pnr);
		debug_print(INFO_LEVEL, debug_buf);
	}
	
	struct sockaddr_in serv_addr;

	if(sock == -1) {
		error("Error opening socket");
	}

	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(pnr);
	

	#if 1
	int ip_ret = inet_pton(AF_INET, ip, &serv_addr.sin_addr.s_addr);
	if (ip_ret != 1) {	
		if (ip_ret == 0) {
			fprintf(stderr, "Invalid IP address: %s\n", ip);
		} else {
			perror("inet_pton()");
		}
		close(sock);
		return -1;	
	}
	#else
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	#endif

	//fang opp og behandle error ved tilkobling av socket
	int optval = 1;
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int))) {
		close(s);
		error("Error setsockopt()");
	}

	if(bind(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
		close(s);
		error("Error on binding");
	}

    if(debug_mode ==1) {
		sprintf(debug_buf, "Bound successfully to %s:%d!", ip, pnr);
		debug_print(INFO_LEVEL, debug_buf);
    }
	
	//fang opp og behandle error
	if(listen(s, SOMAXCONN)) {
		perror("Error listening");
		close(s);
		return -1;
	}
	
	return s;
}

/***
 * Metoden får inn en socket, og lytter på tilkoblinger til denne. Den vil deretter 
 * akseptere klienten når denne kobler til, og returnere den. Kilder brukt for å 
 * implementere denne metoden ligger i README.txt.
 *
 * input:
 *     int socket : Tar inn en socket.
 * returverdi:
 *     int client_s : Returnerer en klientsocket.
 ***/
int accept_connect(int sock) {
	struct sockaddr_in client_addr;
	socklen_t s_len = sizeof(client_addr);
	memset(&client_addr, 0, sizeof(client_addr));
	
	//printf("Waiting for connection");
	int client_s = accept(sock, (struct sockaddr *)&client_addr, &s_len);
	
	if(debug_mode ==1) {
		sprintf(debug_buf, "Waiting for connection to %d", client_s);
		debug_print(INFO_LEVEL, debug_buf);
    }
	if(client_s < 0) {
		close(sock);
		error("Error no accept");
	}
	
	char* client_ip = inet_ntoa(client_addr.sin_addr);
	uint32_t client_ip_bin = ntohl(client_addr.sin_addr.s_addr);
//	printf("Client connected from IP/port: %s/0x%x\n", client_ip, client_ip_bin);
	if(debug_mode ==1) {
		sprintf(debug_buf, "Client connected from IP/port: %s/0x%x\n", client_ip, client_ip_bin);
		debug_print(INFO_LEVEL, debug_buf);
    }
	close(sock);
	return client_s;
}


/***
 * Kalles med argumenter:
 * ./server <ip> <port> <jobbliste>
 * eller
 * ./server <ip> <port> <jobbliste> d 1
 ***/
int main (int argc, char* argv[]) {
	
	if(usage(argc, argv)) {
		return EXIT_SUCCESS;
	}
	
	if(strcmp(argv[3], "d")) {
		debug_mode = 1;
		signal(SIGINT, ctrl_chandler);
	
		//åpne job-filen
		fp = fopen(argv[3], "r");
		if (fp == NULL){
       		printf("Error opening file!");
       		exit(1);
		}
	
		//starte tilkobling til klient
		char ip[100];
		get_ip_addr(argv[1], ip);
	
		sock = create_socket(ip, argv[2]);
		sprintf(debug_buf, "creating socket %d", sock);
		debug_print(INFO_LEVEL, debug_buf);
		if (sock == -1) {
			error("Error create_socket()");
		}

		client_sock = accept_connect(sock);
		sprintf(debug_buf, "client accepting socket %d", sock);
		debug_print(INFO_LEVEL, debug_buf);
		if (client_sock == -1) {
			error("Error accept_connect()");
		}
	
		while(1) {
			char buffer[256] = { 0 };
			ssize_t ret = recv(client_sock, buffer, sizeof(buffer)-1, 0);
			sprintf(debug_buf, "Message from client: %s", buffer);
			debug_print(INFO_LEVEL, debug_buf);

			char mld = buffer[0];
		
			if(ret == 0) {
				printf("Client disconnected. Exiting...\n");
				break;
			} else if(ret == -1) {
				if(errno == EINTR) {
					return 0;
				}
			
				perror("Error reciving message");
				close(sock);
				return -1;
			} else if(mld == 'G') {	
				jobb new_job = get_job();
				if (new_job.error) {
					ret = send(client_sock, "q", strlen("q"), 0);
					printf("No more jobs available!\n");
					
					// Avslutningsmelding fra klient.
					ret = recv(client_sock, buffer, sizeof(buffer)-1, 0);
					if (strcmp(buffer, "T") == 0) {	
						printf("Client disconnected normally.\nExiting...\n");
						break;
					}
				} else {
					unsigned char len = new_job.len;
					printf("new_job.type: %c\n", new_job.type);
					printf("new_job.length: %0x\n", len);
					char job[new_job.len+2];
					job[0] = new_job.type;
					job[1] = new_job.len;

					for (int i = 2; i <= len+1; i++) {
						job[i] = new_job.txt[i-2];
					}
					printf("Job to send : %s \n", job);
					ret = send(client_sock, job, new_job.len+2, 0);
				}	
			} else if(mld == 'E') {
				printf("Client terminated from exception.\n");
				break;
			} else if(mld == 'T') {
				printf("Client disconnected. Exiting...\n");
				break;
			}
		
			if(ret == -1) {
				if(errno == EINTR) {
					return 0;
				}
				perror("Error sending");
				close(sock);
				return -1;		
			}
		}
	
		fclose(fp);
		close(client_sock);
		close(sock);
	} else {
		signal(SIGINT, ctrl_chandler);
	
		//åpne job-filen
		fp = fopen(argv[3], "r");
		if (fp == NULL){
    	   printf("Error opening file!");
 	      exit(1);
		}
	
		//starte tilkobling til klient
		char ip[100];
		get_ip_addr(argv[1], ip);
	
		sock = create_socket(ip, argv[2]);
		printf("%d", sock);
		if (sock == -1) {
			error("Error create_socket()");
		}

		client_sock = accept_connect(sock);
		printf("%d", client_sock);
		if (client_sock == -1) {
			error("Error accept_connect()");
		}
	
		while(1) {
			char buffer[256] = { 0 };
			ssize_t ret = recv(client_sock, buffer, sizeof(buffer)-1, 0);
			printf("Message from client: %s\n", buffer);
			char mld = buffer[0];
		
			if(ret == 0) {
				printf("Client disconnected. Exiting...\n");
				break;
			} else if(ret == -1) {
				if(errno == EINTR) {
					return 0;
				}
			
				perror("Error reciving message");
				close(sock);
				return -1;
			} else if(mld == 'G') {	
				jobb new_job = get_job();
				if (new_job.error) {
					ret = send(client_sock, "q", strlen("q"), 0);
					printf("No more jobs available!\n");
					
					// Avslutningsmelding fra klient.
					ret = recv(client_sock, buffer, sizeof(buffer)-1, 0);
					if (strcmp(buffer, "T") == 0) {	
						printf("Client disconnected normally.\nExiting...\n");
						break;
					}
				} else {
					unsigned char len = new_job.len;
					printf("new_job.type: %c\n", new_job.type);
					printf("new_job.length: %0x\n", len);
					char job[new_job.len+2];
					job[0] = new_job.type;
					job[1] = new_job.len;

					for (int i = 2; i <= len+1; i++) {
						job[i] = new_job.txt[i-2];
					}
					printf("Job to send : %s \n", job);
					ret = send(client_sock, job, new_job.len+2, 0);
				}	
			} else if(mld == 'E') {
				printf("Client terminated from exception.\n");
				break;
			} else if(mld == 'T') {
				printf("Client disconnected. Exiting...\n");
				break;
			}
		
			if(ret == -1) {
				if(errno == EINTR) {
					return 0;
				}
				perror("Error sending");
				close(sock);
				return -1;		
			}
		}
	
		fclose(fp);
		close(client_sock);
		close(sock);
	}
	return 0;
}