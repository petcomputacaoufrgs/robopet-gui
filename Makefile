CC = colorgcc

ROBOPET_PATH = ../lib
ROBOPET_LIB = $(ROBOPET_PATH)/robopet.a
INCLUDE_PATH = .

COMMUNICATION_PATH = ../communication/
COMMUNICATION_H = $(COMMUNICATION_PATH)/packets
COMMUNICATION_LIB = $(COMMUNICATION_PATH)/communication.a
SOCKETS_PATH = $(COMMUNICATION_PATH)/socket

FLAGS = -Wall -I$(ROBOPET_PATH) -I$(COMMUNICATION_H) -I$(SOCKETS_PATH) -I$(INCLUDE_PATH) -lprotobuf `pkg-config --libs --cflags gtk+-2.0` `pkg-config --libs --cflags protobuf`


OBJ_NAMES = mainWindow interface guiPathplan game drawing displaySettings communication
OBJ_FILES = $(addsuffix .o,$(OBJ_NAMES))
OBJECTS = $(addprefix $(OBJDIR)/,$(OBJ_FILES))

OBJDIR = obj
BIN = gui


all: main.cpp $(OBJECTS)
	@$(CC) $^ -o $(BIN) $(FLAGS) $(ROBOPET_LIB) $(COMMUNICATION_LIB)
	@echo "Finished GUI Make."

$(OBJDIR)/%.o: %.cpp %.h | $(OBJDIR)
	@echo $@
	@$(CC) -c $< -o $@ $(FLAGS)

$(OBJDIR):
	@echo "Created OBJ directory."
	@mkdir $(OBJDIR) -p


clean:
	rm -f gui
	rm -f -r $(OBJDIR)
