CXX = g++
CXXFLAGS = -std=c++17 -Wall \
  -I$(shell brew --prefix portaudio)/include \
  -I$(shell brew --prefix jsoncpp)/include \
  -I$(shell brew --prefix openssl)/include \
  -I$(shell brew --prefix libsndfile)/include \
  $(shell pkg-config --cflags opencv4)  # <-- Modifica chiave qui

LDFLAGS = \
  -L$(shell brew --prefix portaudio)/lib \
  -L$(shell brew --prefix jsoncpp)/lib \
  -L$(shell brew --prefix openssl)/lib \
  -L$(shell brew --prefix libsndfile)/lib

LDLIBS_RECORDER = -lportaudio -lsndfile
LDLIBS_SEND = -lcurl -ljsoncpp -lssl -lcrypto \
              $(shell pkg-config --libs opencv4)  # <-- Modifica chiave qui

all: recorder send

recorder: audioRecorder.o
	$(CXX) $< -o $@ $(LDFLAGS) $(LDLIBS_RECORDER)

send: audioSend.o
	$(CXX) $< -o $@ $(LDFLAGS) $(LDLIBS_SEND)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o recorder send