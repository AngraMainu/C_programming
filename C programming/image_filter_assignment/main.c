#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned char *ReadImg(char *argv[]);

void FillBorders(unsigned char *img_sharp, unsigned char *img_orig);

void Sharpen(unsigned char* img_orig, unsigned char* img_sharp);

/* Y = round(0.2126*R + 0.7152*G + 0.0722*B) */
void GreyScale_toTXT(unsigned char* img_sharp);

void SaveToPPM(unsigned char *img);

int IMG_W = 0, IMG_H = 0, COLOR_MAX = 0;

int INTERVAL[5] = { 0 };

const short kernel[3][3] = {{0, -1, 0}, 
                          {-1,  5, -1},
                           {0, -1, 0}};


int main(int argc, char *argv[]){

    /*      CODE        */

    /*FIRST: reading image into array*/
    unsigned char *img_orig = ReadImg(argv);

    /*SECOND: creating image with the same borders*/
    unsigned char *img_sharp = malloc( (IMG_W*IMG_H*3) * (sizeof(unsigned char)));
    FillBorders(img_sharp, img_orig);

    /*THIRD: applying the sharpening filter*/
    Sharpen(img_orig, img_sharp);

    /*FOURTH: creating the output.txt (gray shade)*/
    GreyScale_toTXT(img_sharp);
    
    /*FIFTH: converting array into output image ppm*/
    SaveToPPM(img_sharp);
    free(img_orig);
    free(img_sharp);
    return 0;
}


/*Checking if image is PPM and reading it's size*/

unsigned char *ReadImg(char *argv[]){

    char img_format[2], *ppm_format = "P6";
    char *filename = argv[1];
    FILE *f = fopen(filename, "rb");

    if(!f) {
        fprintf(stderr, "Can't open file %s\n", filename);
        exit(100);
    }

    if(fscanf(f, "%s", img_format) != 1) {
        fprintf(stderr, "Can't read file format!\n");
        fclose(f);
        exit(101);
        if(strcmp(ppm_format, img_format) != 0){
            fprintf(stderr, "Image format is not PPM!\n");
            fclose(f);
            exit(101);
        }
    }

    if(fscanf(f, "%d\n %d\n %d\n", &IMG_W, &IMG_H, &COLOR_MAX) != 3) {
        fprintf(stderr, "Error reading image size!\n");
        fclose(f);
        exit(102);
    }
    
    unsigned char *img = malloc((IMG_W*IMG_H*3)*sizeof(unsigned char));
    if(fread(img, sizeof(unsigned char), IMG_W*IMG_H*3, f) == 0) {
        fprintf(stderr, "Error reading bytes inside file %s\n", filename);
        fclose(f);
        exit(103);
    }
    

    fclose(f);
    return img;
}

/*Duplicates border pixels of the original image*/

void FillBorders(unsigned char *img_sharp, unsigned char *img_orig) {

    for (int i = 0; i < IMG_W; i++) {
        for (int j = 0; j < 3; j++) {

            img_sharp[ i*3+j ] = img_orig[ i*3+j ];
            
            img_sharp[ IMG_W*(IMG_H-1)*3 + i*3+j ] = img_orig[ IMG_W*(IMG_H-1)*3 + i*3+j ];
        }

    }
    for (int i = 0; i < IMG_H; i++) {
        for (int j = 0; j < 3; j++) {

            img_sharp[ IMG_W*i*3 + j ] = img_orig[ IMG_W*i*3 + j ];

            img_sharp[ (IMG_W*(i+1)*3-3) + j ] = img_orig[ (IMG_W*(i+1)*3-3) + j ];
            
        }
    }
}

/*Applies the sharpening mask*/

void Sharpen(unsigned char* img_orig, unsigned char* img_sharp){
    int start = ( (IMG_W+1)*3 );
    int end = (IMG_W*(IMG_H-1)*3);
    int skip_value = 2;
    int tmp = 0;

    for (int i = 0; (start + i) < end; i+=3) {

        /*SKIP BOUNDARIES CHECK*/
            if(( (start+i) == (IMG_W*skip_value*3)) || ( (start+i) == ( (IMG_W*skip_value*3) - 3))){
            i+=3;
            skip_value+=1;
            continue;
        } else {

            /*RED*/
            tmp = kernel[0][1] * img_orig[ (start+i) - IMG_W*3] +
                           kernel[1][0] * img_orig[ (start+i) - 3]       +
                           kernel[1][1] * img_orig[start+i]              +
                           kernel[1][2] * img_orig[ (start+i) + 3]       +
                           kernel[2][1] * img_orig[ (start+i) + IMG_W*3];
            img_sharp[start+i] = tmp <= 0 ? 0 : (tmp >= 255 ? 255 : tmp);

            /*GREEN*/
            tmp = kernel[0][1] * img_orig[ (start+i +1) - IMG_W*3] +
                           kernel[1][0] * img_orig[ (start+i +1) - 3]       +
                           kernel[1][1] * img_orig[start+i +1]              +
                           kernel[1][2] * img_orig[ (start+i +1) + 3]       +
                           kernel[2][1] * img_orig[ (start+i +1) + IMG_W*3];
            img_sharp[start+i +1 ] = tmp <= 0 ? 0 : (tmp >= 255 ? 255 : tmp);
            /*BLUE*/
            tmp = kernel[0][1] * img_orig[ (start+i +2) - IMG_W*3] +
                           kernel[1][0] * img_orig[ (start+i +2) - 3]       +
                           kernel[1][1] * img_orig[start+i +2]              +
                           kernel[1][2] * img_orig[ (start+i +2) + 3]       +
                           kernel[2][1] * img_orig[ (start+i +2) + IMG_W*3];
            img_sharp[start+i +2 ] = tmp <= 0 ? 0 : (tmp >= 255 ? 255 : tmp);
        }
    }
    
}

/*Counts pixel intervals in grey scale*/

void GreyScale_toTXT(unsigned char* img_sharp){

    float grey_scale = 0;

    for (int i = 0; i < IMG_W*IMG_H*3; i+=3)
    {
        grey_scale =  0.2126 * img_sharp[ i ] + 0.7152 * img_sharp[ i+1 ] + 0.0722 * img_sharp[ i+2 ];

        grey_scale = round(grey_scale);

        if(grey_scale <= 50){
            INTERVAL[0]++;
        }
        else if (grey_scale >= 51 && grey_scale <= 101)
        {
            INTERVAL[1]++;
        }
        else if (grey_scale >= 102 && grey_scale <= 152)
        {
            INTERVAL[2]++;
        }
        else if (grey_scale >= 153 && grey_scale <= 203){
            INTERVAL[3]++;
        }
        else if (grey_scale >= 204)
        {
            INTERVAL[4]++;
        }
    }

    FILE *f = fopen("output.txt", "w");
    fprintf(f,"%d %d %d %d %d\n", INTERVAL[0], INTERVAL[1], INTERVAL[2], INTERVAL[3], INTERVAL[4]);
    fclose(f);
}

/*Saves array into ppm format image*/

void SaveToPPM(unsigned char *img){
    FILE *f = fopen("output.ppm", "wb");
    if(!f) {
        fprintf(stderr, "Can't write to file!\n");
        exit(100);
    }
    

    fprintf(f, "P6\n%d\n%d\n%d\n", IMG_W, IMG_H, COLOR_MAX);
    fwrite(img, sizeof(unsigned char), IMG_W*IMG_H*3, f);

    fclose(f);
}