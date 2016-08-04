from PIL import Image
import sys




path_to_file = sys.argv[1]

in_img = Image.open(path_to_file).convert("RGBA")
img = Image.new('RGBA',in_img.size)
in_pix = in_img.load()
pix = img.load()


for x in range(img.size[0]):
   for y in range(img.size[1]):
      pixel = in_pix[x, y]
      if sum(pixel) != 0:
         pixel = (pixel[0], 255 - pixel[1], pixel[2], pixel[3])
      pix[x, y] = pixel

img.save(path_to_file, 'PNG')
