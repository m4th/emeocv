.PHONY: clean, mrproper

PROJECT = emeocv

OBJS = $(addprefix $(OUTDIR)/,\
  Directory.o \
  Config.o \
  ImageProcessor.o \
  ImageInput.o \
  KNearestOcr.o \
  Plausi.o \
  RRDatabase.o \
  main.o \
  )

CC = g++
CFLAGS = -Wno-write-strings -I .

# DEBUG
ifneq ($(RELEASE),true)
CFLAGS += -g -D _DEBUG
OUTDIR = Debug
else
OUTDIR = Release
endif

BIN := $(OUTDIR)/$(PROJECT)

LDLIBS = -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core  -lrrd -llog4cpp

SUFFIXES= .cpp .o
.SUFFIXES: $(SUFFIXES) .


all: $(BIN)

$(OUTDIR):
	mkdir $(OUTDIR)
	
$(OBJS): $(OUTDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN) : $(OUTDIR) $(OBJS)
	$(CC) $(CFLAGS) $(LDFALGS) $(OBJS) $(LDLIBS) -o $(BIN)

.cpp.o:
	$(CC) $(CFLAGS) -c $*.cpp

clean:
	rm -rf $(OUTDIR)/*.o

mrproper: clean
	rm -rf $(BIN)
