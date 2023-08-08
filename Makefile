SRC := $(foreach x, ./src, $(wildcard $(addprefix $(x)/*,.c*)))
DIRS := $(foreach x, ./src/**, $(wildcard $(addprefix $(x)/*,.c*)))
INCLUDES:=$(shell pkg-config --cflags libavformat libavcodec libswresample libswscale libavutil gtk+-3.0)
LIBS=$(shell pkg-config --libs gtk+-3.0 )
out.o: 
	gcc ${SRC} -I include ${INCLUDES} ${DIRS} -o build/krokodil -pthread -ldiscord -lcurl ${LIBS} && ./build/krokodil