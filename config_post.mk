default: ${OBJ}
	${CC} ${LDFLAGS} ${OBJ} -o ${NAME}.so

clean:
	rm -f ${OBJ} ${NAME}.so

.c.o:
	${CC} -fPIC ${CFLAGS} -c $< -o $(<:.c=.o)

.cpp.o:
	${CC} -lstdc++ -fPIC ${CFLAGS} -c $< -o $(<:.cpp=.o)

install:
	install ${NAME}.so ${PLUG_DIR}
