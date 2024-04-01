#include "image.h"
#include <stdio.h>
#include <stdlib.h>

Image *load_ppm(const char *filename)
{
  int i, j;
  Image *img;

  FILE *fp = fopen(filename, "r");
  if (!fp)
  {
    perror("File opening failed");
    return NULL;
  }

  img = malloc(sizeof(Image));
  if (fscanf(fp, "P3\n%d %d\n%d\n", &img->width, &img->height,
             &img->max_color) != 3)
  {
    fclose(fp);
    free(img);
    return NULL;
  }

  img->pixels = malloc(img->height * sizeof(int **));
  for (i = 0; i < img->height; i++)
  {
    img->pixels[i] = malloc(img->width * sizeof(int *));
    for (j = 0; j < img->width; j++)
    {
      img->pixels[i][j] = malloc(3 * sizeof(int));
      if (fscanf(fp, "%d %d %d", &img->pixels[i][j][0], &img->pixels[i][j][1],
                 &img->pixels[i][j][2]) != 3)
      {
        printf("Error reading pixel at (%d, %d)\n", i, j);
        fclose(fp);
        free(img);
        return NULL;
      }
    }
  }
  printf("Loaded image with width=%d, height=%d\n", img->width, img->height);

  fclose(fp);
  return img;
}

void save_ppm(const char *filename, Image *img)
{
  int i, j;
  FILE *fp = fopen(filename, "w");
  if (!fp)
  {
    perror("File opening failed");
    return;
  }

  fprintf(fp, "P3\n%d %d\n%d\n", img->width, img->height, img->max_color);
  for (i = 0; i < img->height; i++)
  {
    for (j = 0; j < img->width; j++)
    {
      fprintf(fp, "%d %d %d ", img->pixels[i][j][0], img->pixels[i][j][1],
              img->pixels[i][j][2]);
    }
    fprintf(fp, "\n");
  }

  fclose(fp);
}

void adjust_brightness(Image *img, int change)
{
  int i, j, k, new_value;
  for (i = 0; i < img->height; i++)
  {
    for (j = 0; j < img->width; j++)
    {
      /* R, G, B */
      for (k = 0; k < 3; k++)
      {
        new_value = img->pixels[i][j][k] + change;
        if (new_value > img->max_color)
          new_value = img->max_color;
        if (new_value < 0)
          new_value = 0;
        img->pixels[i][j][k] = new_value;
      }
    }
  }
}

void adjust_temperature(Image *img, int change)
{
  int i, j, k, new_value;
  for (i = 0; i < img->height; i++)
  {
    for (j = 0; j < img->width; j++)
    {
      /* R, G, B */
      for (k = 0; k < 3; k++)
      {
        if (k == 0)
        {
          new_value = img->pixels[i][j][k] + change;
          if (new_value > img->max_color)
            new_value = img->max_color;
          if (new_value < 0)
            new_value = 0;
          img->pixels[i][j][k] = new_value;
        }
        else if (k == 2)
        {
          new_value = img->pixels[i][j][k] - change;
          if (new_value > img->max_color)
            new_value = img->max_color;
          if (new_value < 0)
            new_value = 0;
          img->pixels[i][j][k] = new_value;
        }
      }
    }
  }
}

void adjust_tint(Image *img, int change)
{
  int i, j, k, new_value;
  for (i = 0; i < img->height; i++)
  {
    for (j = 0; j < img->width; j++)
    {
      /* R, G, B */
      for (k = 0; k < 3; k++)
      {
        if (k == 0)
        {
          new_value = img->pixels[i][j][k] + change;
          if (new_value > img->max_color)
            new_value = img->max_color;
          if (new_value < 0)
            new_value = 0;
          img->pixels[i][j][k] = new_value;
        }
        else if (k == 1)
        {
          new_value = img->pixels[i][j][k] - change;
          if (new_value > img->max_color)
            new_value = img->max_color;
          if (new_value < 0)
            new_value = 0;
          img->pixels[i][j][k] = new_value;
        }
      }
    }
  }
}

void free_image(Image *img)
{
  int i, j;
  for (i = 0; i < img->height; i++)
  {
    for (j = 0; j < img->width; j++)
    {
      free(img->pixels[i][j]);
    }
    free(img->pixels[i]);
  }
  free(img->pixels);
  free(img);
}
