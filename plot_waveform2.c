#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

    // Calculate the number of samples
    uint32_t numSamples = header.dataSize / (header.bitsPerSample / 8);

    FILE *plotFile = fopen("waveform_data.txt", "w");
    if (!plotFile) {
        perror("Error creating plot data file");
        fclose(file);
        return;
    }

    // Read and write data in chunks
    int16_t buffer[4096]; // Buffer to hold 4096 samples at a time
    uint32_t samplesRead = 0;
    while (samplesRead < numSamples) {
        uint32_t samplesToRead = (numSamples - samplesRead > 4096) ? 4096 : numSamples - samplesRead;
        fread(buffer, sizeof(int16_t), samplesToRead, file);
        for (uint32_t i = 0; i < samplesToRead; ++i) {
            fprintf(plotFile, "%f %d\n", (float)(samplesRead + i) / header.sampleRate, buffer[i]);
        }
        samplesRead += samplesToRead;
    }

    fclose(plotFile);
    fclose(file);

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

