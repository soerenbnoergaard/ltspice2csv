all : ltspice2csv 

ltspice2csv : ltspice2csv.c
	gcc ltspice2csv.c -o ltspice2csv

install :
	cp ltspice2csv /usr/local/bin

uninstall : 
	rm /usr/local/bin/ltspice2csv
