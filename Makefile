CC = clang
FLAGS = -Wall -Wextra

all: starter myls mymv mycp

starter: starter.c
		${CC} ${WFLAGS} -o starter starter.c

myls: myls.c
		${CC} ${WFLAGS} -o myls myls.c

mymv: mymv.c
		${CC} ${WFLAGS} -o mymv mymv.c

mycp: mycp.c
		${CC} ${WFLAGS} -o mycp mycp.c

clean:
		rm -f myls starter mymv mycp