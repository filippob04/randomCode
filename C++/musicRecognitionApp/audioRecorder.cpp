#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <cmath>
#include <vector>

#include <portaudio.h>
#include <sndfile.h>

// Costanti necessarie per Pa_Stream
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

// Struct per audio
struct audioData {
    std::vector<float> samples;
    int channels;
};

// Funzione checkErr che verifica la tipologia di errore presentato in input
static void checkErr(PaError err){
    if(err != paNoError){
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        exit(EXIT_FAILURE);
    }
}

// Memorizzazione della traccia audio
static int patestCallback(
    const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, 
    const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData
){
    audioData* data = (audioData*)userData;
    const float* in = (const float*)inputBuffer;
    (void)outputBuffer;

    if(!in){return paContinue;}
    for(unsigned long i = 0; i < framesPerBuffer * data->channels; i++){
        data->samples.push_back(in[i]);
    }

    return paContinue;
}

int main(){
    // Gestisco eventuali errori
    PaError err;
    err = Pa_Initialize();
    checkErr(err);

    // Verifico la presenza di un dispositivo audio
    int numDevices = Pa_GetDeviceCount();
    printf("Number of devices: %d\n", numDevices);

    if(numDevices <= 0){
        printf("Error getting device count.\n");
        Pa_Terminate();
        exit(EXIT_FAILURE);
    } 

    // Ottengo le Informazioni di ciascun dispositivo
    const PaDeviceInfo* deviceInfo;
    for(int i = 0; i < numDevices; i++){
        deviceInfo = Pa_GetDeviceInfo(i);
        printf("Device %d:\n", i);
        printf("    name%s\n", deviceInfo->name);
        printf("    maxInputChannels%d\n", deviceInfo->maxInputChannels);
        printf("    maxOutputChannels%d\n", deviceInfo->maxOutputChannels);
        printf("    defaultSampleRate%f\n", deviceInfo->defaultSampleRate);
    }

    // Scelgo quale dispositivo Input utilizzare
    int inputDevice = 2;
    const PaDeviceInfo* inputDeviceInfo = Pa_GetDeviceInfo(inputDevice);

    // Imposto i parametri del dispositivo di Input
    PaStreamParameters inputParameters;
    memset(&inputParameters, 0, sizeof(inputParameters));
    inputParameters.channelCount = inputDeviceInfo->maxInputChannels;
    inputParameters.device = inputDevice;
    inputParameters.hostApiSpecificStreamInfo =  NULL;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = inputDeviceInfo->defaultLowInputLatency;

    audioData data;
    data.channels = inputParameters.channelCount;

    // Inizializzo l'AudioStream
    PaStream* stream;
    err = Pa_OpenStream(
        &stream,
        &inputParameters,
        nullptr, // No Output
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paNoFlag,
        patestCallback,
        &data
    );
    checkErr(err);

    err = Pa_StartStream(stream);
    checkErr(err);

    printf("Recording for 10 seconds...\n");
    Pa_Sleep(10*1000); // Dieci secondi di cattura

    // Termino portAudio
    err = Pa_StopStream(stream);
    checkErr(err);
    err = Pa_CloseStream(stream);
    checkErr(err);

    err = Pa_Terminate();
    checkErr(err);

    // Salvataggio file WAV
    SF_INFO sfinfo;
    sfinfo.channels = data.channels;
    sfinfo.samplerate = SAMPLE_RATE;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; // Formato audio necessario a 16bit

    SNDFILE* outfile = sf_open("output.wav", SFM_WRITE, &sfinfo);
    if(!outfile){
        fprintf(stderr, "Error opening WAV file!\n");
        return 1;
    }

    // Converto float -> int16
    std::vector<short> intSamples(data.samples.size());
    for(size_t i = 0; i < data.samples.size(); ++i){
        float s = data.samples[i];
        if(s > 1.0f) s = 1.0f;
        if(s < -1.0f) s = -1.0f;
        intSamples[i] = static_cast<short>(s * 32767);
    }

    sf_count_t written = sf_write_short(outfile, intSamples.data(), intSamples.size());
    if(written != (sf_count_t)intSamples.size()){
        fprintf(stderr, "Error writing samples to WAV!\n");
    }
    sf_close(outfile);

    printf("Recording saved to output.wav\n");
    return 0;
} 