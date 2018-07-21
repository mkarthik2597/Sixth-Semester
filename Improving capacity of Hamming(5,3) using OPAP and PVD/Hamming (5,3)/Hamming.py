import numpy as np
from math import floor
from PIL import Image
import matplotlib.pyplot as plt
from sys import argv


def str_to_bin(string):
    """ Take a string and return a binary representation of the string based
        upon the ASCII values of each character """
    # Creates a list of elements. Here, the list consists of individual characters
    ret = list(string)

    # ord('x') converts 'x' to ASCII
    # convert to binary representation. Each element is a 7-bit ASCII representation.

    ret = ['{:07b}'.format(ord(x)) for x in ret]
    # Each element now is a list of the 7-bit ASCII representation of each character
    ret = [[bit for bit in x] for x in ret]
    # Merge all the sublists to a single list. Meanwhile, convert each string to an integer
    ret = [int(bit) for sublist in ret for bit in sublist]
    return ret

def bin_to_str(binary):
    """ take a list of binary (integer ones and zeros) and return its ascii
        string representation """
    output = []
    for i in range(int(floor(len(binary)/7))):
        start = i * 7
        # this is gross
        char = binary[start:start+7]
        char = '0b' + ''.join([str(c) for c in char])
        output.append(int(char, 2))
    #print output
    st = ''.join([chr(o) for o in output if o != 0])
    return str(st)

def convert_msg(msg):
    """ Takes a list of binary numbers and groups them into triples """
    while len(msg) % 3 != 0:
        # pad the message to length
        msg.append(0)
    bin_strings = [msg[3*i:3*i+3] for i in range(len(msg)/3)]
    return bin_strings

def convert_lsb(lsb):
    bin_strings = [lsb[5*i:5*i+5] for i in range(len(lsb)/5)]
    return bin_strings

def toDec(binary):
    mult_arr = 2**np.arange(3)[::-1]
    ans = np.dot(binary,mult_arr)
    return int(ans)

def toBinary(decimal):
    ret = []
    while decimal >= 1:
        ret.append(decimal % 2)
        decimal = decimal / 2;

    while len(ret)!=8:
        ret.append(0)

    return list(reversed(ret))


script,image=argv
msg=open("../input.txt","r").read()

print "Reading message from input file..."
raw_input()
msg = str_to_bin(msg)
print("")
print("Message in binary:")
print np.asarray(msg)
print("")
# Wait for user to enter a character
raw_input()
print("Message grouped into 3 bits each (5,3)Hamming Code:")
print("")
msg = convert_msg(msg)
# Print the array with atmost 10 elements.
print np.asarray(msg)
raw_input()

# Open image using PIL and convert it into a numpy array. Each element of the array contains
# the pixel intensity value
CoverImage = Image.open("../CoverImages/%s.bmp"%image).convert('L')
CoverImage = np.asarray(CoverImage)
CoverImage.setflags(write=1)

#create a copy of the image
ReferenceImage = CoverImage.copy()

# Without specifying vmin and vmax, plt.imshow auto-adjusts its range to the min and max of the data.
plt.imshow(CoverImage,cmap='gray',vmin=0,vmax=255)
# The title is displayed just on top of the image at the centre
plt.title("Original Image")
# Display the image
plt.show()
print("")
print("Extracting LSBs from Original Image ...")
# lsb is an array where each element is (pixel_intensity)%2
lsb = np.mod(CoverImage,2)
# Reshape the array into a single dimensional array
# Convert the ndarray to a python list
lsb = lsb.reshape(1,-1).tolist()[0]
raw_input()
print("Group into bits of 5:")
lsb = convert_lsb(lsb)
print np.asarray(lsb)

print("")
# Returns the height and width of the image
H,W = CoverImage.shape

count = (H * W)/5

# The number of 3 bits secret data should be atmost the number of 5-pixel groups
if(len(msg)>len(lsb)):
    raise ValueError("Message too long to hide. Aborting....")

while(len(msg)!=len(lsb)):
    msg.append([0,0,0])

# The secret data now contains 3 bits for every 5 pixels of the image
len_msg_bits = len(msg)*3

msg = np.asarray(msg)
lsb = np.asarray(lsb)

# CheckMatrix is the parity check matrix
CheckMatrix =  np.mat('1,0,1,0,0;\
                       0,1,0,1,0;\
                       1,1,0,0,1')
raw_input()
print("The check matrix...")
print(CheckMatrix)
print("")

# A dictionary with keys as syndromes and values as the location of 1-bits in the corresponding error vector
lookup ={ 0 : [],
          1 : [4],
          2 : [3],
          4 : [2],
          3 : [1],
          5 : [0],
          6 : [0,1],
          7 : [1,2],
          }
raw_input()
print("")
print("Applying the algorithm specified in the paper ...")

CoverImage = CoverImage.flatten()
lsb_list = lsb.flatten()
# For each group of 5 pixels
for i in range(len(msg)):
    # r: A vector of 5 pixels
    lsbVector = lsb[i]
    # delta: 3 bits of secret image
    msgVector = msg[i]

    Syndrome = np.dot(lsbVector,CheckMatrix.transpose())
    # Sxor = syndrome XOR delta
    newSyndrome = np.mod(Syndrome + msgVector, 2)
    # Convert Sxor into decimal
    decSyndrome = toDec(np.asarray(newSyndrome))
    # Flip bits
    for bit in lookup[decSyndrome]:
        pixelPosition = 5*i + bit

        if(lsb_list[pixelPosition]):
            CoverImage[pixelPosition] -= 1
        else:
            CoverImage[pixelPosition] += 1

CoverImage = CoverImage.reshape(H,W)
lsb = lsb.reshape(H,W)

new_img = Image.fromarray(CoverImage)
new_img.save("../StegoImages/%s.bmp" %("stego_"+image))

plt.imshow(new_img,cmap='gray',vmin=0,vmax=255)
plt.title("Stego Image")
plt.show()

new_lsb = np.mod(CoverImage,2)
new_lsb = np.asarray(new_lsb)
msg_bits = new_lsb.reshape(-1,5)[:len(msg)]

RecoveredBits = np.mod(np.dot(msg_bits, CheckMatrix.transpose()),2)
print np.asarray(RecoveredBits)

raw_input()
bin_msg_bits = RecoveredBits.reshape(1,-1).tolist()[0]
output_msg = bin_to_str(bin_msg_bits)

print "Writing to output file..."
open("../output.txt","w").write(output_msg)
raw_input()

print "Verifying correctness of recovered message..."
raw_input()
if open("../output.txt","r").read() == open("../input.txt","r").read():
    print "Message received correctly!"
else:
    print "Error in receipt of message!"

CoverImgTemp = CoverImage.astype(np.int16)
ReferenceImgTemp = ReferenceImage.astype(np.int16)
raw_input()
print("Running calculations...")
total_pixels = H*W
change = CoverImgTemp - ReferenceImgTemp

mean_sqr_error = np.sum((change)**2)/float(total_pixels)
PSNR = 10*np.log10(total_pixels/mean_sqr_error)
embedding_rate = float(len_msg_bits)/total_pixels
non_zero = np.count_nonzero(change)
change_density = float(non_zero)*100/ total_pixels

raw_input()

print "{:25}".format("PSNR"), PSNR
print "{:25}".format("Embedding Rate"), embedding_rate
print "{:25} {}%".format("Change Density",change_density)
print "{:25}".format("Capacity"), len_msg_bits
