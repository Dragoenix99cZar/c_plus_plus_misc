// #include <stdio.h>
// #include <stdlib.h>

// int main() {
//     // 1. Open the file for writing in text mode
//     FILE *fp = fopen("output.ppm", "w");
//     if (fp == NULL) {
//         perror("Error opening file");
//         return EXIT_FAILURE;
//     }

//     // 2. Define image dimensions
//     int width = 3;
//     int height = 2;
//     int max_color = 255;

//     // 3. Write the PPM header
//     fprintf(fp, "P3\n");
//     fprintf(fp, "%d %d\n", width, height);
//     fprintf(fp, "%d\n", max_color);

//     // 4. Write pixel data (R G B)
//     // Row 1: Red, Green, Blue
//     fprintf(fp, "255 0 0    0 255 0    0 0 255\n");
    
//     // Row 2: Yellow, Black, White
//     fprintf(fp, "255 255 0  0 0 0      255 255 255\n");

//     // 5. Close the file
//     fclose(fp);
//     printf("PPM image generated successfully as 'output.ppm'.\n");

//     return EXIT_SUCCESS;
// }

// #include <stdio.h>

// int main() {
//     FILE *fp = fopen("gradient.ppm", "w");
//     int width = 256;
//     int height = 256;

//     fprintf(fp, "P3\n%d %d\n255\n", width, height);

    // for (int y = 0; y < height; y++) {
    //     for (int x = 0; x < width; x++) {
    //         int r = x;         // Red increases horizontally
    //         int g = y;         // Green increases vertically
    //         int b = 128;       // Constant blue
    //         fprintf(fp, "%d %d %d ", r, g, b);
    //     }
    //     fprintf(fp, "\n");     // Newline at the end of each pixel row
    // }

//     fclose(fp);
//     return 0;
// }

// ffmpeg -i gradient.ppm gradient_ppm2jpg.jpg





//------------------------------------------------------------------
//--------------------------Piped Version----------------------------
//------------------------------------------------------------------
#include <stdio.h>

int main() {
    int width = 1024;
    int height = 1024;

    // Write the PPM header to stdout
    fprintf(stdout, "P3\n%d %d\n255\n", width, height);

    // Write pixel data to stdout
    // Row 1: Red, Green, Blue
    // fprintf(stdout, "255 0 0    0 255 0    0 0 255\n");
    // Row 2: Yellow, Black, White
    // fprintf(stdout, "255 255 0  0 0 0      255 255 255\n");

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int r = (x / (float)width) * 255;         // Red increases horizontally
            int g = (y / (float)height) * 255;         // Green increases vertically
            int b = 128;       // Constant blue
            fprintf(stdout, "%d %d %d ", r, g, b);
        }
        fprintf(stdout, "\n");     // Newline at the end of each pixel row
    }

    return 0;
}

// gcc ppm_test.c -o ppm_run.exe
// ppm_run.exe | ffmpeg -f image2pipe -vcodec ppm -i - pipOut.jpg