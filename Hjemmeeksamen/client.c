// Biblioteker
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <ctype.h> 
#include <signal.h>

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

// Definisjoner
#define STDIN 'O'
#define STDOUT 'E'

// Globale variabler
int sock, fd_stdout[2], fd_stderr[2];
long wait_time = 1000;

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
 * implementere denne metoden ligger i Kilder.txt.
 *
 * input:
 *		int argc: Tar inn antall argumenter.
 *		char* argv[]: Tar inn array med argumentene.
 * return:
 *		int : Returnerer 0 dersom brukeren ikke har sendt inn riktig antall argumenter.
 ***/
int usage(int argc, char* argv[]) {
    if(argc != 3 || strcmp(argv[3], "d")) {
        printf("NORMAL USAGE: %s <ip> <port> \n", argv[0]);
        //Andre debug muligheter vil bli lagt til dersom det blir tid for det.
        printf("DEBUG USAGE: %s <port> d <mode 0/1 (off, info)>", argv[0]);
        return 1;
    }
    return 0;
}

/***
 * Metoden tar inn en error melding som så skrives ut til stderr. Kilder brukt for å 
 * implementere denne metoden ligger i Kilder.txt.
 * 
 * input:
 * 	char* msg: error meldingen
 ***/
void error(char* msg) {
	if(debug_mode ==1) {
		sprintf(debug_buf, "Error occured");
		debug_print(INFO_LEVEL, debug_buf);
    }
    
    perror(msg);
    exit(EXIT_FAILURE);
}

/***
 * Metoden sender en beskjed til serveren som består av én-bytes meldinger. Endre til maks
 * 4 byte.
 *
 * input:
 *	char* msg: en streng som skal sendes til serveren.
 ***/
void send_msg(char msg[]) {
	ssize_t ret = 0;
	if (msg[0] == 'G') {
		if(debug_mode ==1) {
			sprintf(debug_buf, "Sending message %c", msg[0]);
			debug_print(INFO_LEVEL, debug_buf);
    	}
		// Dette forhindrer at klienten kan sende ulovlige kommandoer til serveren.
		char tmp[1] = {'G'};
		ret = send(sock, tmp, strlen(tmp), 0);
	} else if (msg[0] == 'T') {
		if(debug_mode ==1) {
			sprintf(debug_buf, "Sending message %c", msg[0]);
			debug_print(INFO_LEVEL, debug_buf);
    	}
		ret = send(sock, "T", strlen("T"), 0);
	} else if (msg[0] == 'E') {
		if(debug_mode ==1) {
			sprintf(debug_buf, "Sending message %c", msg[0]);
			debug_print(INFO_LEVEL, debug_buf);
    	}
		ret = send(sock, "E", strlen("T"), 0);
	}
	
	if(debug_mode ==1) {
		sprintf(debug_buf, "Message %c sent", msg[0]);
		debug_print(INFO_LEVEL, debug_buf);
    }
	
	if (ret == -1) {
		close(sock);
		error("Error sending message");
	}
}

/***
 * Metoden avslutter programmet på riktig måte, og forteller server at klient avsluttet 
 * på riktig måte. Lukker socket og pipes, for så å avslutte.
 ***/
void quit() {
	if(debug_mode == 0) { //debug-modus
		send_msg("T");
		sprintf(debug_buf, "Exiting, sending message to server");
		debug_print(INFO_LEVEL, debug_buf);
		
		sprintf(debug_buf, "closing socket %d", sock);
		debug_print(INFO_LEVEL, debug_buf);
		close(sock); //stenger socket
		
		sprintf(debug_buf, "closing fd_Stdout and stderr");
		debug_print(INFO_LEVEL, debug_buf);
		//stenger write-pipene
		close(fd_stdout[1]);
		close(fd_stderr[1]);
	
		exit(0); //Avslutter
	} else { // normal
		printf("Exiting...");
		send_msg("T");
	
		close(sock); //stenger socket
	
		//stenger write-pipene
		close(fd_stdout[1]);
		close(fd_stderr[1]);
	
		exit(0); //Avslutter
	}
}

/***
 * Avslutter programmet om brukeren trykker ctrl+c (^c)
 * input:
 *     int sig: signal
 ***/
void ctrl_chandler(int sig) {
	signal(sig, SIG_IGN);
	quit();
}

/***
 * Metoden mottar en melding fra serveren og returnerer meldingen.
 *
 * return:
 *		char* new_msg: char-peker av teksten sendt fra server med nullterminering.
 */
char* recv_msg() {
	char* msg = malloc(sizeof(char)*256);
	char* new_msg;
	ssize_t ret;
	ret = recv(sock, msg, 256, 0);
	
	if(msg[0] != 'q') {
		if(debug_mode ==1) {
			sprintf(debug_buf, "Message recieved %c", msg[0]);
			debug_print(INFO_LEVEL, debug_buf);
    	}
		unsigned char len = msg[1]+3;
		new_msg = malloc(sizeof(char)*len);
		
		for(int i = 0; i < len; i++) {
			new_msg[i] = msg[i];
		}
		
		new_msg[len-1] = 0;
	} else {
		new_msg = malloc(sizeof(char));
		new_msg[0] = 'q';
		if(debug_mode ==1) {
			sprintf(debug_buf, "New message %c", new_msg[0]);
			debug_print(INFO_LEVEL, debug_buf);
    	}
	}
	
	free(msg);
	
	if (ret == 0) {
		printf("Server disconnected. \n");
	} else if (ret == -1) {
		close(sock);
		error("Error with recv()");
	}
	
	return new_msg;
}

/***
 * Metoden tar inn en string og returnerer en array med ip-addressen. Kilder brukt for å
 * implementere denne metoden ligger i Kilder.txt.
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
 * Metoden henter en jobb fra serveren og printer ut denne til enten stdout eller stderr.
 ***/
void get_job() {
	char tmp[1] = {'G'};
	char* r_msg;
	
	send_msg(tmp);
	r_msg = recv_msg();
	char target = r_msg[0];
	
	// Sørger for at forelderen ikke avslutter før barneprosessene.
	usleep(wait_time);
	
	if (r_msg[0] == 'q') {
		printf("No more available jobs!\n");
		free(r_msg);
		quit();
	}else {
		unsigned char len = r_msg[1];
		char to_write[len+1];
		int ch_sum = 0; 
		
		//Summere alle karakterer og ta modulo av summen for å finne checksum
		for(int i = 2; i < len+1; i++) { //hopper over de første bytene (jobbinfo & lengde)
		//	printf("%c \t", to_write[i]);
			ch_sum += r_msg[i];
		//  printf("%d \t", to_write[i]);
		}
		
		div_t res = div(ch_sum, 32);
		int res_ch = res.quot;
		
		//få tak i bitene 0-4 i jobbinfo
		int check = r_msg[0] << 4;
		
		
	   /* 
		* Hvis checksum stemmer overens med jobbens checksum skal jobben skrives til
		* stdout eller stderr.
	    */
		if(res_ch == check) {
			
			// Fjerner jobbtype og lengde
			for (int i = 0; i < len+1; i++) {
				to_write[i] = r_msg[i+2];
			}
		
			// Legger til 10µsekunder til ventetiden.
			wait_time += 10;
			if (target == STDIN) {
				write(fd_stdout[1], to_write, len);
			} else if (target == STDOUT) {
				write(fd_stderr[1], to_write, len);
			} else if (r_msg[0] == 0) {
				// Dette forhindrer kræsj om man ikke mottar en melding
			} else {
				printf("Job type '%c' is unsupported!\n", target);
			}
		} else {
			printf("Checksum didn't match. \n");
			
		}
	}
	
	free(recv_msg);
}

/***
 * Kilder brukt for å implementere denne metoden ligger i Kilder.txt.
 * input:
 *		char ip: IPv4-adresse til serveren man vil koble til.
 *		char* port: Port serveren ligger i.
 * return:
 *		int s: socket til serveren.
 ***/
int create_socket(char ip[], char* port) {
	int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(debug_mode == 1) {
		sprintf(debug_buf, "Socket created %d", s);
		debug_print(INFO_LEVEL, debug_buf);
	}	
	
	int pnr = atoi(port);
	if(debug_mode == 1) {
		sprintf(debug_buf, "IP-address %d in use", pnr);
		debug_print(INFO_LEVEL, debug_buf);
	}
		
	struct sockaddr_in serv_addr;
	if(sock == -1) {
		error("Error opening socket");
	}
	
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(pnr);
	
	int ip_ret = inet_pton(AF_INET, ip, &serv_addr.sin_addr.s_addr);
	if (ip_ret != 1) {
		if (ip_ret == 0) {
			close(s);
			error("Invalid IP address");
		} else {
			close(s);
			error("inet_pton()");
		}
	}
		
	if(debug_mode == 1) {
		sprintf(debug_buf, "Connecting to %s: %d", ip, pnr);
		debug_print(INFO_LEVEL, debug_buf);
	}
		
	if(connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
		close(s);
		error("Error connecting");
	}
		
	return s;
}

/***
 * Metode håndterer kommandoer fra bruker, og tar forespørsler via get_job() til 
 * serveren.
 ***/
void commands() {
	printf("What do you wish to do? \n");
	printf("\tG - Get new job from server \n");
	printf("\tA - Get all jobs from server \n");
	printf("\tX - Get x amount jobs from server \n");
	printf("\tQ - Quit program \n");
	
	while(1) {
		//forhindre meldingen som barneprosessen printer ut fra å bli avbrutt av kommandoløkken
		sleep(1);
		
		//lengden på meldinger er to bytes
		char msg[2] = {0};
		
		scanf("%s", msg);
		if(msg[0] == 'Q' || msg[0] == 'q') {
			quit();
		} else if(msg[0] == 'G' || msg[0] == 'g') {
			get_job();
		} else if(msg[0] == 'A' || msg[0] == 'a') {
			//sender en og en "hent jobb"-melding
			while(1) {
				get_job();
			}			
		} else if(msg[0] == 'X' || msg[0] == 'x') {
			/* 
			   slette alle tegn fra stdin-bufferet, slik at tidligere input ikke
			   videreføres. Kilde for denne delen ligger i kilder.txt
			*/
			char clear;
			while ((clear = getchar()) != '\n' && clear != EOF) {}
			 
			/////////////////////////////////////////////////////
			printf("How many jobs? \n");		
			char ant;
			scanf("%c", &ant);
			
			///Sender en og en "hent jobb"-melding
			for(int i = 0; i < ant; i++) {
				get_job();
			}
			
		} else {
			printf("Invalid command");
		}
		
		char clear;
		while ((clear = getchar()) != '\n' && clear != EOF) {}
	}
}

int main(int argc, char* argv[]) {
	if(usage(argc, argv)) {
		return EXIT_SUCCESS;
	}

	// Debug mode	
	if(strcmp(argv[2], "d")) {
		debug_mode = 1;		
		sprintf(debug_buf, "debug level: %d", debug_mode);
		debug_print(INFO_LEVEL, debug_buf);
		
		signal(SIGINT, ctrl_chandler);
		
		if(pipe(fd_stdout) == -1 || pipe(fd_stderr) == -1) {
			error("Error pipe()");
		}
	
		pid_t child_out = fork();
		//fang opp evt. errorer.
		if(child_out == -1) {
			error("Error forking.");
		} else if(child_out == 0) { //Barneprosess for stdout
			//stenger unødvendige read og write pipes
			close(fd_stderr[0]); // Read
			close(fd_stderr[1]); // Write
			close(fd_stdout[1]); // Write
		
			char buffer[256];
			while(read(fd_stdout[0], &buffer, 1) > 0) {
				write(STDOUT_FILENO, &buffer, 1);
			}
		
			printf("Child_stdout terminating... \nPipe closed.\n");
			close(fd_stdout[0]); //stenger read
			exit(0); // Terminerer så barneprosessen ikke kan forsette i programflyten.
		}
	
		pid_t child_err = fork();
		//fang opp evt. errorer
		if(child_err == -1) {
			error("Error forking.");
		} else if(child_err == 0) { //Barneprosess for stderr
			//stenger unødvendige read og write pipes
			close(fd_stdout[0]); // Read
			close(fd_stdout[1]); // Write
			close(fd_stderr[1]); // Write
		
			char buffer[256];
			while(read(fd_stderr[0], &buffer, 1) > 0) {
				write(STDERR_FILENO, &buffer, 1);
			}
		
			printf("Child_stderr terminating... \nPipe closed.\n");
			close(fd_stderr[0]); //stenger read
			exit(0); // Terminerer så barneprosessen ikke kan forsette i programflyten.
		
		} else { //Foreldreprosessen
			//Stenger read pipes
			close(fd_stderr[0]);
			close(fd_stdout[0]);
		
			char tmp[100];
			char *port = argv[1];
	
			get_ip_addr(argv[1], tmp);
			
			sock = create_socket(tmp, port);
			sprintf(debug_buf, "Socket created, number: %d\n", sock);
    		debug_print(INFO_LEVEL, debug_buf);
			printf("Connected to file server!\n");
			
			commands();
			quit();
		}
	// Normal
	} else { 
	
		signal(SIGINT, ctrl_chandler);
	
		if(pipe(fd_stdout) == -1 || pipe(fd_stderr) == -1) {
			error("Error pipe()");
		}
	
		pid_t child_out = fork();
		//fang opp evt. errorer.
		if(child_out == -1) {
			error("Error forking.");
		} else if(child_out == 0) { //Barneprosess for stdout
			//stenger unødvendige read og write pipes
			close(fd_stderr[0]); // Read
			close(fd_stderr[1]); // Write
			close(fd_stdout[1]); // Write
		
			char buffer[256];
			while(read(fd_stdout[0], &buffer, 1) > 0) {
				write(STDOUT_FILENO, &buffer, 1);
			}
		
			printf("Child_stdout terminating... \nPipe closed.\n");
			close(fd_stdout[0]); //stenger read
			exit(0); // Terminerer så barneprosessen ikke kan forsette i programflyten.
		}
	
		pid_t child_err = fork();
		//fang opp evt. errorer
		if(child_err == -1) {
			error("Error forking.");
		} else if(child_err == 0) { //Barneprosess for stderr
			//stenger unødvendige read og write pipes
			close(fd_stdout[0]); // Read
			close(fd_stdout[1]); // Write
			close(fd_stderr[1]); // Write
		
			char buffer[256];
			while(read(fd_stderr[0], &buffer, 1) > 0) {
				write(STDERR_FILENO, &buffer, 1);
			}
		
			printf("Child_stderr terminating... \nPipe closed.\n");
			close(fd_stderr[0]); //stenger read
			exit(0); // Terminerer så barneprosessen ikke kan forsette i programflyten.
		
		} else { //Foreldreprosessen
			//Stenger read pipes
			close(fd_stderr[0]);
			close(fd_stdout[0]);
		
			char tmp[100];
			char *port = argv[2];
	
			get_ip_addr(argv[1], tmp);
		
			sock = create_socket(tmp, port);
			printf("Connected to file server!\n");
			commands();
			quit();
		}
	}
	return 0;
}