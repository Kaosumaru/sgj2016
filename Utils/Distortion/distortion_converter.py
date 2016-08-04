import Image

def vectorToRGBA(vec, divider = 5):
   x1 = x2 = y1 = y2 = 0.0

   if vec[0] < 0:
      x2 = -vec[0]
   else:
      x1 = vec[0] 

   if vec[1] < 0:
      y2 = -vec[1]
   else:
      y1 = vec[1] 

   
   return (int(x2*255.0)/divider, int(x1*255.0)/divider, int(y2*255.0)/divider, int(y1*255.0)/divider)


def RGToVec(RGBA):
   r = float(RGBA[0])/256.0
   g = float(RGBA[1])/256.0
   x = (0.5 - r);
   y = -(0.5 - g);
   return (x, y)


in_path = "C:\Python27\Distortion\distortion_test.png"

in_img = Image.open(in_path)
img = Image.new('RGBA',in_img.size)
in_pix = in_img.load()
pix = img.load()


for x in range(img.size[0]):
   for y in range(img.size[1]):
      vec = RGToVec(in_pix[x, y])
      color = vectorToRGBA(vec)
      pix[x, y] = color

img.save('distortion1.png', 'PNG')

