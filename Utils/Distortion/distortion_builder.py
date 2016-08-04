import Image

def vectorToRGBA(vec):
   x1 = x2 = y1 = y2 = 0

   if vec[0] < 0:
      x2 = -vec[0]
   else:
      x1 = vec[0] 

   if vec[1] < 0:
      y2 = -vec[1]
   else:
      y1 = vec[1] 
   
   return (x2, x1, y2, y1)

img = Image.new('RGBA',(256, 256))
pix = img.load()

for x in range(img.size[0]):
   for y in range(img.size[1]):
      pix[x, y] = (20, 0, 0, 0)

img.save('distortion2.png', 'PNG')

