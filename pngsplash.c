#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <gd.h>

#define MIN(a, b) (a < b ? a : b)

int writeFb(gdImagePtr img, int img_width, int img_height) {
  int fd = open("/dev/fb0", O_RDWR);
  struct fb_var_screeninfo screeninfo;
  int bitspp, bytespp, fb_width, fb_height, d_width, d_height;
  unsigned int *fb_data;

  ioctl(fd, FBIOGET_VSCREENINFO, &screeninfo);
  bitspp = screeninfo.bits_per_pixel;

  if (bitspp != 32) {
    close(fd);
    return 1;
  }

  fb_width  = screeninfo.xres;
  fb_height = screeninfo.yres;
  bytespp = bitspp / 8;

  if(sizeof(unsigned int) != bytespp) {
    close(fd);
    return 1;
  }

  d_width = MIN(fb_width, img_width);
  d_height = MIN(fb_height, img_height);

  fb_data = (unsigned int*) mmap(0, fb_width * fb_height * bytespp, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  for (int y = 0; y < d_height; y++) {
    for (int x = 0; x < d_width; x++) {
      fb_data[y * fb_width + x] = gdImageGetPixel(img, x, y);
    }
  }

  munmap(fb_data, fb_width * fb_height * bytespp);
  close(fd);

  return 0;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: pngimage <background_image>\n");
    return 1;
  }

  gdImagePtr bg_img;
  int bg_width, bg_height;

  bg_img = gdImageCreateFromFile(argv[1]);
  bg_width = gdImageSX(bg_img);
  bg_height = gdImageSY(bg_img);

  if (writeFb(bg_img, bg_width, bg_height)) {
    printf("Could not write the image to the frame buffer.");
  }

  gdImageDestroy(bg_img);
  return 0;
}
