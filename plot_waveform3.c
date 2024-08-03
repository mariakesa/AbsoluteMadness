#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// WAV header structure
struct WAVHeader {
    char riff[4];         // RIFF Header
    uint32_t chunkSize;   // RIFF Chunk Size
    char wave[4];         // WAVE Header
    char fmt[4];          // FMT header
    uint32_t subchunk1Size; // Size of the fmt chunk
    uint16_t audioFormat; // Audio format
    uint16_t numChannels; // Number of channels
    uint32_t sampleRate;  // Sampling Frequency
    uint32_t byteRate;    // Bytes per second
    uint16_t blockAlign;  // Bytes per sample
    uint16_t bitsPerSample; // Bits per sample
};

struct LISTChunk {
    char chunkID[4];      // "LIST"
    uint32_t size;        // Size of the sub-chunk less 8
    char listTypeID[4];   // List type ID
    // Data follows depending on list type ID, which we will skip
};

void plot_waveform(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    struct WAVHeader header;
    fread(&header, sizeof(header), 1, file);

    // Check for LIST chunk and skip it if present
    char chunkID[4];
    uint32_t chunkSize;

    while (fread(chunkID, sizeof(char), 4, file) == 4) {
        fread(&chunkSize, sizeof(uint32_t), 1, file);
        if (strncmp(chunkID, "LIST", 4) == 0) {
            // Skip the LIST chunk
            fseek(file, chunkSize, SEEK_CUR);
        } else if (strncmp(chunkID, "data", 4) == 0) {
            break;
        } else {
            // Skip unknown chunks
            fseek(file, chunkSize, SEEK_CUR);
        }
    }

    // Allocate memory for the audio data based on chunkSize
    uint16_t *data = malloc(chunkSize);
    if (data == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return;
    }

    // Read the audio data
    fread(data, chunkSize, 1, file);
    fclose(file);

    FILE *plotFile = fopen("waveform_data.txt", "w");
    if (!plotFile) {
        perror("Error creating plot data file");
        free(data);
        return;
    }

    // Calculate the number of samples
    uint32_t numSamples = chunkSize / (header.bitsPerSample / 8);

    // Write the audio data to the plot file
    for (uint32_t i = 0; i < numSamples; ++i) {
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

