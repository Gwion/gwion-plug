all: ${NAME}.so

${NAME}.so: ${OBJ}
ifeq (${USE_LD}, 1)
	${LD} $^ -o ${NAME}.so ${LDFLAGS}
else
	${CC} $^ -o ${NAME}.so ${LDFLAGS}
endif

clean:
	rm -f ${OBJ} ${NAME}.so

.c.o:
	${CC} -fPIC ${CFLAGS} -c $< -o $(<:.c=.o)

.cpp.o:
	${CC} -lstdc++ -fPIC ${CFLAGS} -c $< -o $(<:.cpp=.o)

install: ${NAME}.so
	install ${NAME}.so ${PLUG_DIR}

uninstall:
	rm ${PLUG_DIR}/${NAME}.so
