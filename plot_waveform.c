#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// WAV header structure
struct WAVHeader {
    char riff[4];        // RIFF Header
    uint32_t chunkSize;  // RIFF Chunk Size
    char wave[4];        // WAVE Header
    char fmt[4];         // FMT header
    uint32_t subchunk1Size; // Size of the fmt chunk
    uint16_t audioFormat;   // Audio format
    uint16_t numChannels;   // Number of channels
    uint32_t sampleRate;    // Sampling Frequency
    uint32_t byteRate;      // Bytes per second
    uint16_t blockAlign;    // Bytes per sample
    uint16_t bitsPerSample; // Bits per sample
    // LIST chunk structure
    char listID[4];      // LIST header ID
    uint32_t listSize;   // Size of the LIST chunk
    char listType[4];    // LIST type ID
    char data[4];        // "data" string
    uint32_t dataSize;   // Size of the data section
};

void plot_waveform(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    struct WAVHeader header;
    fread(&header, sizeof(header), 1, file);
  
    uint16_t *data = malloc(header.dataSize);
    fread(data, header.dataSize, 1, file);
    fclose(file);

    FILE *plotFile = fopen("waveform_data.txt", "w");
    if (!plotFile) {
        perror("Error creating plot data file");
        free(data);
        return;
    }
    
    printf("Header chunk size: %d\n",header.chunkSize);
    printf("Data size: %d\n",header.dataSize);
    printf("Size of SubChunk1Size %d\n", header.subchunk1Size);
    printf("boom %s\n", header.data);
    printf("size %s\n", header.listType);

    for (uint32_t i = 0; i < header.dataSize / 2; ++i) {
        fprintf(plotFile, "%f %d\n", (float)i / header.sampleRate, data[i]);
    }
    fclose(plotFile);

    free(data);

    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (!gnuplot) {
        perror("Error opening gnuplot");
        return;
    }

    fprintf(gnuplot, "set title 'Waveform of %s'\n", filename);
    fprintf(gnuplot, "set xlabel 'Time (s)'\n");
    fprintf(gnuplot, "set ylabel 'Amplitude'\n");
    fprintf(gnuplot, "plot 'waveform_data.txt' with lines\n");
    pclose(gnuplot);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <wavfile>\n", argv[0]);
        return 1;
    }

    plot_waveform(argv[1]);
    return 0;
}

