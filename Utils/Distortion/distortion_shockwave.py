import Image
import math

def vectorToRGBA(vec, divider = 5, test = False):
   x1 = x2 = y1 = y2 = 0.0

   if vec[0] < 0:
      x2 = -vec[0]
   else:
      x1 = vec[0] 

   if vec[1] < 0:
      y2 = -vec[1]
   else:
      y1 = vec[1] 

   if test:
      return (int(x2*255.0)/divider, int(x1*255.0)/divider, int(y2*255.0)/divider, 255)
   
   return (int(x2*255.0)/divider, int(x1*255.0)/divider, int(y2*255.0)/divider, int(y1*255.0)/divider)


class Vec3:
   def __init__(self, x=0, y=0, z=0):
      self.x = x
      self.y = y
      self.z = z

   def __getitem__(self, index):
      return [self.x, self.y, self.z][index];

   
def distance_between(v1, v2):
   return math.sqrt((v1[0] - v2[0]) ** 2 + (v1[1] - v2[1]) ** 2)

def normalize(v):
   l = distance_between(Vec3(), v)
   v.x = v.x / l
   v.y = v.y / l

img = Image.new('RGBA',(256, 256))
pix = img.load()


center = Vec3(img.size[0]/2,img.size[1]/2)
time = 80.0
shockParams = Vec3(10.0, 0.8, 7)

for x in range(img.size[0]):
   for y in range(img.size[1]):
      uv = Vec3(x,y)
      distance = distance_between(uv, center)
      if distance <= time + shockParams.z and distance >= time - shockParams.z:
         diff = (distance - time)
         powDiff = 1.0 - math.fabs(diff*shockParams.x)**shockParams.y
         diffTime = diff  * powDiff

         diffUV = Vec3(uv.x, uv.y)
         diffUV.x = center.x - diffUV.x 
         diffUV.y = diffUV.y - center.y
         normalize(diffUV)

         #diffUV.x = diffUV.x * diffTime / 30.0
         #diffUV.y = diffUV.y * diffTime / 30.0
         rgba = vectorToRGBA(diffUV, 5)
         pix[x, y] = tuple(int(i * diffTime / 40) for i in rgba)
         #diffUV = normalize(uv - center);

img.save('distortion_shock.png', 'PNG')

