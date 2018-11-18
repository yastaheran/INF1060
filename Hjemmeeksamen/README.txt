Denne hjemmeeksamen inneholder en mappe med jobbfiler, to c-filer, en makefile og to
tekstfiler. Protokoll.txt er beskrivelse av hvordan jeg har designet meldingsformidlingen mellom server
og klient. C-filene er min implementering av løsning.

Jeg har ikke fått sjekket valgrind fordi jeg ikke får det til å fungere som det skal på
macen, og jeg hadde ikke mulighet til å dra til universitet og kjøre valgrind på en av
ifi sine datamaskiner. Det er en del problemer når man kjører koden og debug-delen ble 
ikke ferdig derfor er det en debug-funksjon i begge c-filene og ingen header fil.

Under er en oversikt over alle kilder jeg har brukt for å implementere løsningen. Kildene 
er sortert etter først filnavn, deretter metodenavn.

Client.c
	commands()
		http://stackoverflow.com/questions/7898215/how-to-clear-input-buffer-in-c.
	create_socket()
		inspirert av kode fra plenumstime uke 11 og 12
	ctrl_chandler()
		man 2 signal
		https://www.tutorialspoint.com/c_standard_library/c_function_signal.htm
		plenumstimene
	usage()
		hentet fra plenumstime uke 12

Debugger.c
	hentet fra plenumstime uke 13

Debugger.h
	hentet fra plenumstime uke 13
	
Server.c
	accept_connect()
		inspirert av kode fra plenumstime uke 11 og 12
		http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/server.c		
	create_socket()
		http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/server.c
		kode fra plenumstime
	ctrl_chandler()
		man 2 signal
		https://www.tutorialspoint.com/c_standard_library/c_function_signal.htm
		plenumstimene
	error(char* msg)
		hentet fra http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/server.c
	get_ip_addr()
		inspirert av http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/server.c og 
		http://www.binarytides.com/hostname-to-ip-address-c-sockets-linux/
	usage()
		hentet fra plenumstime uke 12

Debugger.h
	hentet fra plenumstime uke 13