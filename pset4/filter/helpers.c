#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    //loop through the image array
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            //take the average of the RGB values
            float average = ((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            int gray = round(average);
            //convert the pixel color to the average gray amount
            image[i][j].rgbtBlue = gray;
            image[i][j].rgbtGreen = gray;
            image[i][j].rgbtRed = gray;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //getting the value of half of the widths array length
    int halfWidth = floor(width/2);

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < halfWidth; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width-1-j];
            image[i][width-1-j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //making a copy of the image to maintan the original values of the image to comparison
    RGBTRIPLE copy[height][width];

    //looping throught the image values to create the blur image
    for(int h = 0; h < height; h++)
    {
        for(int w = 0; w < width; w++)
        {
            int avgBlue = 0;
            int avgGreen = 0;
            int avgRed = 0;
            float avgCounter = 0.0;

            //create a 3x3 block
            for(int hh = -1; hh < 2 ; hh++)
            {
                for(int ww = -1; ww < 2; ww++)
                {
                    //check to see if the pixel of the block is not out of bounds
                    if(hh + h < 0 || ww + w < 0 || hh + h >= height || ww + w >= width)
                    {
                        continue;
                    }
                    else
                    {
                        avgBlue += image[h + hh][w + ww].rgbtBlue;
                        avgGreen += image[h + hh][w + ww].rgbtGreen;
                        avgRed +=  image[h + hh][w + ww].rgbtRed;
                        avgCounter++;
                    }

                }
            }

            //assign average to the image copy (this doesnt modify the value of the image itself)
            copy[h][w].rgbtBlue = round(avgBlue / avgCounter);
            copy[h][w].rgbtGreen = round(avgGreen / avgCounter);
            copy[h][w].rgbtRed = round(avgRed / avgCounter);
        }
    }

    //passing al the new values from the copy to the image
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
     //making a copy of the image to maintan the original values of the image to comparison
    RGBTRIPLE copy[height][width];

    //looping through the image values to check the edges of the image
    for(int h = 0; h < height; h++)
    {
        for(int w = 0; w < width; w++)
        {
            int sumGxBlue = 0;
            int sumGyBlue = 0;
            int sumGxGreen = 0;
            int sumGyGreen = 0;
            int sumGxRed = 0;
            int sumGyRed = 0;

            //create a 3x3 block
            for(int hh = -1; hh < 2 ; hh++)
            {
                for(int ww = -1; ww < 2; ww++)
                {
                    //check to see if the pixel of the block is not out of bounds
                    if(hh + h < 0 || ww + w < 0 || hh + h >= height || ww + w >= width)
                    {
                        continue;
                    }
                    else
                    {
                        //assigning Gx and Gy values based on kernel
                        if(hh == -1 && ww == -1)
                        {
                            sumGxBlue += image[h + hh][w + ww].rgbtBlue * -1;
                            sumGyBlue += image[h + hh][w + ww].rgbtBlue * -1;
                            sumGxGreen += image[h + hh][w + ww].rgbtGreen * -1;
                            sumGyGreen += image[h + hh][w + ww].rgbtGreen * -1;
                            sumGxRed +=  image[h + hh][w + ww].rgbtRed * -1;
                            sumGyRed +=  image[h + hh][w + ww].rgbtRed * -1;
                        }
                       if(hh == -1 && ww == 0)
                        {
                            sumGyBlue += image[h + hh][w + ww].rgbtBlue * -2;
                            sumGyGreen += image[h + hh][w + ww].rgbtGreen * -2;
                            sumGyRed +=  image[h + hh][w + ww].rgbtRed * -2;
                        }
                       if(hh == -1 && ww == 1)
                        {
                            sumGxBlue += image[h + hh][w + ww].rgbtBlue;
                            sumGyBlue += image[h + hh][w + ww].rgbtBlue * -1;
                            sumGxGreen += image[h + hh][w + ww].rgbtGreen;
                            sumGyGreen += image[h + hh][w + ww].rgbtGreen * -1;
                            sumGxRed +=  image[h + hh][w + ww].rgbtRed;
                            sumGyRed +=  image[h + hh][w + ww].rgbtRed * -1;
                        }
                        if(hh == 0 && ww == -1)
                        {
                            sumGxBlue += image[h + hh][w + ww].rgbtBlue * -2;
                            sumGxGreen += image[h + hh][w + ww].rgbtGreen * -2;
                            sumGxRed +=  image[h + hh][w + ww].rgbtRed * -2;

                        }
                        if(hh == 0 && ww == 0)
                        {
                            continue;
                        }
                        if(hh == 0 && ww == 1)
                        {
                            sumGxBlue += image[h + hh][w + ww].rgbtBlue * 2;
                            sumGxGreen += image[h + hh][w + ww].rgbtGreen * 2;
                            sumGxRed +=  image[h + hh][w + ww].rgbtRed * 2;
                        }
                        if(hh == 1 && ww == -1)
                        {
                            sumGxBlue += image[h + hh][w + ww].rgbtBlue * -1;
                            sumGyBlue += image[h + hh][w + ww].rgbtBlue;
                            sumGxGreen += image[h + hh][w + ww].rgbtGreen * -1;
                            sumGyGreen += image[h + hh][w + ww].rgbtGreen;
                            sumGxRed +=  image[h + hh][w + ww].rgbtRed * -1;
                            sumGyRed +=  image[h + hh][w + ww].rgbtRed;
                        }
                        if(hh == 1 && ww == 0)
                        {
                            sumGyBlue += image[h + hh][w + ww].rgbtBlue * 2;
                            sumGyGreen += image[h + hh][w + ww].rgbtGreen * 2;
                            sumGyRed +=  image[h + hh][w + ww].rgbtRed * 2;
                        }
                        if(hh == 1 && ww == 1)
                        {
                            sumGxBlue += image[h + hh][w + ww].rgbtBlue;
                            sumGyBlue += image[h + hh][w + ww].rgbtBlue;
                            sumGxGreen += image[h + hh][w + ww].rgbtGreen;
                            sumGyGreen += image[h + hh][w + ww].rgbtGreen;
                            sumGxRed +=  image[h + hh][w + ww].rgbtRed;
                            sumGyRed +=  image[h + hh][w + ww].rgbtRed;
                        }
                    }

                }
            }

            //assign average to the image copy (this doesnt modify the value of the image itself)
            float resultBlue = sqrt(pow(sumGxBlue, 2) + pow(sumGyBlue, 2));
                if(resultBlue <= 255)
                {
                    copy[h][w].rgbtBlue = round(resultBlue);
                }
                else
                {
                    copy[h][w].rgbtBlue = 255;
                }

            float resultGreen = sqrt(pow(sumGxGreen, 2) + pow(sumGyGreen, 2));
                if(resultGreen <= 255)
                {
                    copy[h][w].rgbtGreen = round(resultGreen);
                }
                else
                {
                    copy[h][w].rgbtGreen = 255;
                }

            float resultRed = sqrt(pow(sumGxRed, 2) + pow(sumGyRed, 2));
                if(resultRed <= 255)
                {
                    copy[h][w].rgbtRed = round(resultRed);
                }
                else
                {
                    copy[h][w].rgbtRed = 255;
                }
        }
    }

    //passing al the new values from the copy to the image
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    return;
}



