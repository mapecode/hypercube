DIREXE := exec/
MCC := mpicc
CC := gcc
RUN := mpirun
NODES := 8
MATH := lm

all: dirs data hypercube new_data solution

dirs:
	mkdir -p $(DIREXE)

data:
	$(CC) gen_data.c -o $(DIREXE)gen_data

hypercube:
	$(MCC) hypercube.c -$(MATH) -o $(DIREXE)hypercube

new_data:
	./$(DIREXE)gen_data $(NODES)

solution :
	$(RUN) -n $(NODES) ./$(DIREXE)hypercube 

clean: 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~ 
	rm datos.dat


