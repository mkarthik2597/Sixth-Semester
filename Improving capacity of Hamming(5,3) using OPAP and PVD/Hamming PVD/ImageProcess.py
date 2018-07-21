from import_modules import *
from helper import *

msg=open("../input.txt","r").read()

msg = str_to_bin(msg)

k=3
msg = convert_msg(msg)

CoverImage = Image.open("barbara.bmp").convert('L')
CoverImage = np.asarray(CoverImage)
ReferenceImage = CoverImage.copy()

CoverImage.setflags(write=1)

#
# plt.imshow(CoverImage,cmap='gray',vmin=0,vmax=255)
# plt.title("Original Image")
# plt.show()

H,W = CoverImage.shape
TotalPixels = H*W

while (len(msg)!=TotalPixels):
    msg.append([0]*k)

CoverImage = CoverImage.flatten()

for i in range(len(msg)):
    CoverImage[i] = CoverImage[i] - CoverImage[i] % 2**k + toDec(msg[i],k)

# raw_input()
CoverImage = CoverImage.reshape(H,W)
new_img = Image.fromarray(CoverImage.reshape(H,W))
new_img.save("stego_barbara.bmp")
#
# plt.imshow(CoverImage,cmap='gray',vmin=0,vmax=255)
# plt.title("Stego Image")
# plt.show()

CoverImgTemp = CoverImage.astype(np.int16)
ReferenceImgTemp = ReferenceImage.astype(np.int16)

DifferenceImage = abs(CoverImgTemp - ReferenceImgTemp)
print np.asarray(DifferenceImage)

print "Min:",DifferenceImage.min(),"Max:",DifferenceImage.max()
ScaledImage = (DifferenceImage - DifferenceImage.min())/(DifferenceImage.min() - DifferenceImage.max())# * 255

print np.asarray(ScaledImage)

# plt.imshow(ScaledImage,cmap='gray',vmin=0,vmax=255)
# plt.title("Scaled Image")
# plt.show()
