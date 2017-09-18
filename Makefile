# for Linux

CC = g++

#CFLAGS  =  -g
#CFLAGS2 =  -c -g
CFLAGS  =  
CFLAGS2 =  -c 


objsuf  = o
objs = Algorithm.$(objsuf) AlgorithmAll.$(objsuf) AlgorithmBigCA.$(objsuf) \
       AlgorithmCA.$(objsuf) AlgorithmGF.$(objsuf) \
       AlgorithmRandom.$(objsuf) AlgorithmXu.$(objsuf) \
       ArrayFile.$(objsuf)  \
       BaseOrthogonalArray.$(objsuf) Constraint.$(objsuf) ModelFile.$(objsuf) \
       Matrix.$(objsuf)  LinearGraph.$(objsuf) Random.$(objsuf)  \
       OrthogonalArray.$(objsuf) ProgInfo.$(objsuf) \
       ExString.$(objsuf) TestParameter.$(objsuf) Util.$(objsuf)

OAG: OAG.cpp $(objs) Matrix.h
	$(CC) $(CFLAGS) OAG.cpp $(objs) \
	-o OAG

Algorithm.$(objsuf): Algorithm.cpp Algorithm.h ProgInfo.h
	$(CC) $(CFLAGS2) Algorithm.cpp

AlgorithmAll.$(objsuf): AlgorithmAll.cpp AlgorithmAll.h ProgInfo.h
	$(CC) $(CFLAGS2) AlgorithmAll.cpp

AlgorithmBigCA.$(objsuf): AlgorithmBigCA.cpp AlgorithmBigCA.h ProgInfo.h
	$(CC) $(CFLAGS2) AlgorithmBigCA.cpp

AlgorithmCA.$(objsuf): AlgorithmCA.cpp AlgorithmCA.h ProgInfo.h
	$(CC) $(CFLAGS2) AlgorithmCA.cpp

AlgorithmGF.$(objsuf): AlgorithmGF.cpp AlgorithmGF.h ProgInfo.h
	$(CC) $(CFLAGS2) AlgorithmGF.cpp

AlgorithmRandom.$(objsuf): AlgorithmRandom.cpp AlgorithmRandom.h ProgInfo.h
	$(CC) $(CFLAGS2) AlgorithmRandom.cpp

AlgorithmXu.$(objsuf): AlgorithmXu.cpp AlgorithmXu.h ProgInfo.h
	$(CC) $(CFLAGS2) AlgorithmXu.cpp

ArrayFile.$(objsuf): ArrayFile.cpp ArrayFile.h ProgInfo.h
	$(CC) $(CFLAGS2) ArrayFile.cpp

BaseOrthogonalArray.$(objsuf): BaseOrthogonalArray.cpp BaseOrthogonalArray.h Matrix.h
	$(CC) $(CFLAGS2) BaseOrthogonalArray.cpp

Constraint.$(objsuf): Constraint.cpp Constraint.h
	$(CC) $(CFLAGS2) Constraint.cpp

ExString.$(objsuf): ExString.cpp ExString.h
	$(CC) $(CFLAGS2) ExString.cpp

Matrix.$(objsuf): Matrix.cpp Matrix.h
	$(CC) $(CFLAGS2) Matrix.cpp

ModelFile.$(objsuf): ModelFile.cpp ModelFile.h Matrix.h
	$(CC) $(CFLAGS2) ModelFile.cpp

ProgInfo.$(objsuf): ProgInfo.cpp ProgInfo.h
	$(CC) $(CFLAGS2) ProgInfo.cpp

LinearGraph.$(objsuf): LinearGraph.cpp LinearGraph.h
	$(CC) $(CFLAGS2) LinearGraph.cpp

OrthogonalArray.$(objsuf): OrthogonalArray.cpp OrthogonalArray.h Matrix.h
	$(CC) $(CFLAGS2) OrthogonalArray.cpp

Random.$(objsuf): Random.cpp Random.h
	$(CC) $(CFLAGS2) Random.cpp

TestParameter.$(objsuf): TestParameter.cpp TestParameter.h
	$(CC) $(CFLAGS2) TestParameter.cpp

Util.$(objsuf): Util.cpp Util.h Matrix.h
	$(CC) $(CFLAGS2) Util.cpp
