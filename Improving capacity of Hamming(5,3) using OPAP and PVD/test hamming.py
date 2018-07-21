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
    bin_strings = [msg[3*i:3*i+3] for i in range(int(len(msg)/3))]
    return bin_strings

def convert_lsb(lsb):
    bin_strings = [lsb[5*i:5*i+5] for i in range(int(len(lsb)/5))]
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


# In[18]:


img = ''
net_psnr =0
net_embed = 0
net_change = 0
net_p = 0
limit = 468

msg=open("../input.txt","r").read()
msg = str_to_bin(msg)
msg = convert_msg(msg)

# In[102]:
for i in range(limit):
    
	print(i)
#

	

	#

	# Wait for user to enter a character



	
	# 
	#


	# Open image using PIL and convert it into a numpy array. Each element of the array contains
	# the pixel intensity value
	CoverImage = Image.open(str(i+1)+".jpg").convert('L')
	CoverImage = np.asarray(CoverImage)
	CoverImage.setflags(write=1)

	#create a copy of the image
	ReferenceImage = CoverImage.copy()




	# lsb is an array where each element is (pixel_intensity)%2
	lsb = np.mod(CoverImage,2)
	# Reshape the array into a single dimensional array
	# Convert the ndarray to a python list
	lsb = lsb.reshape(1,-1).tolist()[0]


	lsb = convert_lsb(lsb)
	#


	# Returns the height and width of the image
	H,W = CoverImage.shape

	count = floor((H * W)/5)

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
	#new_img.save("%s.bmp" %("stego_"+image))



	new_lsb = np.mod(CoverImage,2)
	new_lsb = np.asarray(new_lsb)
	msg_bits = new_lsb.reshape(-1,5)[:len(msg)]

	RecoveredBits = np.mod(np.dot(msg_bits, CheckMatrix.transpose()),2)
	#


	bin_msg_bits = RecoveredBits.reshape(1,-1).tolist()[0]
	output_msg = bin_to_str(bin_msg_bits)

	#
	#open("../"+outfile,"w").write(output_msg)


	# 	


	    

	# DifferenceImage = abs(CoverImage - ReferenceImage)
	# 
	# 
	#
	# diff_img = Image.fromarray(DifferenceImage)
	# diff_img.save("%s.bmp" %("Difference_"+image))
	#
	# plt.imshow(diff_img,cmap='gray',vmin=0,vmax=255)
	# plt.title("Difference Image")
	# plt.show()

	CoverImgTemp = CoverImage.astype(np.int16)
	ReferenceImgTemp = ReferenceImage.astype(np.int16)


	total_pixels = H*W
	change = CoverImgTemp - ReferenceImgTemp

	mean_sqr_error = np.sum((change)**2)/float(total_pixels)
	PSNR = 10*np.log10(total_pixels/mean_sqr_error)
	embedding_rate = float(len_msg_bits)/total_pixels
	non_zero = np.count_nonzero(change)
	change_density = float(non_zero)*100/ total_pixels

	net_psnr +=PSNR
	net_embed += embedding_rate
	net_change += change_density
	net_p += len_msg_bits
        
	
             
net_psnr /= limit
net_embed /= limit
net_change /= limit
net_p /= limit



# In[20]:


print("{:25}".format("PSNR"), net_psnr)
print("{:25}".format("Embedding Rate"), net_embed)
print("{:25}".format("Change Density"),net_change)
print("{:25}".format("Capacity"),net_p)

