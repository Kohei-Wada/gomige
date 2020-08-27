OBJS=gomige.o
SRCS=$(OBJS:%.0=%.c)
CFLAGS=-g -Wall
LDLIBS=
TARGET=gomige
$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)
