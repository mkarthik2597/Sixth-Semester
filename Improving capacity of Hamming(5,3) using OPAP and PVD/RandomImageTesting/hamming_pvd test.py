from helper import *
from import_modules import *


def GenerateBlocks(img,H,W):
    blocks = [];

    for i in range(H):
        if i%2 == 0:
            for j in range(int(W/2)):
                blocks.append([img[i][2*j],img[i][2*j+1]])

            if W%2 and i+1 < H:
                blocks.append([img[i][W-1],img[i+1][W-1]])
        else:
            temp =[]
            for j in range(int(W/2)):
                temp.append([img[i][2*j+1],img[i][2*j]])

            blocks = blocks + list(reversed(temp))

    random.seed(rand_seed)
    RandomSequence = random.sample(range(0, len(blocks)),len(blocks))
    blocks = [blocks[i] for i in RandomSequence]

    return blocks

img = '../images/img'
net_psnr =0
net_embed = 0
net_change = 0
net_p = 0
limit = 468



for ip in range(limit):
	msg=open("../input.txt","r").read()
	msg = str_to_bin(msg)
	msg = convert_msg(msg)

	print(ip)


	# Display Image
	CoverImage = Image.open("../RandomImages/"+str(ip+1)+".jpg").convert('L')
	CoverImage = np.asarray(CoverImage)
	CoverImage.setflags(write=1)


	ReferenceImage = CoverImage.copy()

	H,W = CoverImage.shape

	RangeWidth = [8,8,16,32,64,128]
	quantisation_table = [[0,7],[8,15],[16,31],[32,63],[64,127],[128,255]]

	#RangeWidth = [2,2,4,4,4,8,8,16,16,32,32,64,64]
	#quantisation_table = [[0,1],[2,3],[4,7],[8,11],[12,15],[16,23],[24,31],[32,47],[48,63],[64,95],[96,127],[128,191],[192,255]]








	def ReconstructImage(new_block,H,W):
		blocks = [None] * len(new_block)

		random.seed(rand_seed)
		RandomSequence = random.sample(range(0, len(blocks)),len(blocks))

		j = 0
		for i in range(len(new_block)):
			blocks[RandomSequence[j]] = new_block[i]
			j+=1

		block_index=0
		for i in range(H):
			if i%2 == 0:
				for j in range(int(W/2)):
					CoverImage[i][2*j] = blocks[block_index][0]
					CoverImage[i][2*j+1] = blocks[block_index][1]
					block_index+=1

				if W%2 and i+1 < H:
					CoverImage[i][W-1] = blocks[block_index][0]
					CoverImage[i+1][W-1] = blocks[block_index][1]
					block_index+=1
			else:
				temp = list(reversed(blocks[block_index:block_index+int(W/2)]))

				for j in range(int(W/2)):
					CoverImage[i][2*j+1] = temp[j][0]
					CoverImage[i][2*j] = temp[j][1]
					block_index+=1

		return CoverImage

	def OPAP(original_pixel,modified_pixel,k):
		delta = modified_pixel - original_pixel

		if (delta > 2**(k-1)) and (delta < 2**k):
			if modified_pixel >= 2**k:
				OPAP_pixel = modified_pixel - 2**k
			else:
				OPAP_pixel = modified_pixel

		elif (delta >= -2**(k-1)) and (delta <= 2**(k-1)):
			OPAP_pixel = modified_pixel

		elif (delta > -2**k) and (delta < -2**(k-1)):
			if modified_pixel < 256 - 2**k:
				OPAP_pixel = modified_pixel + 2**k
			else:
				OPAP_pixel = modified_pixel

		return OPAP_pixel


	blocks = GenerateBlocks(CoverImage,H,W)




	PixelDifference = [abs(int(blocks[i][1])-int(blocks[i][0])) for i in range(len(blocks))]




	lsb_list = []




	for i in range(len(PixelDifference)):
		for j in range(len(quantisation_table)):
			if PixelDifference[i] >= quantisation_table[j][0] and PixelDifference[i] <= quantisation_table[j][1]:
				break

		X = min(quantisation_table[j][1] - PixelDifference[i],PixelDifference[i] - quantisation_table[j][0])

		if X < 4:
			bits = 1
		else:
			bits = int(math.floor(math.log(X,2))) - 1

		lsb0 = (blocks[i][0] % (2**bits))
		lsb0 = toBinary(lsb0,bits)
		lsb_list += lsb0

		lsb1 = (blocks[i][1] % (2**bits))
		lsb1 = toBinary(lsb1,bits)
		lsb_list += lsb1

	valid_lsb_list = convert_lsb(lsb_list)
	encodedBits = len(lsb_list) - (len(lsb_list) % 5)







	if(len(msg)>len(valid_lsb_list)):
		raise ValueError("Message too long to hide. Aborting....")

	while(len(msg)!=len(valid_lsb_list)):
		msg.append([0,0,0])

	len_msg_bits = len(msg)*3

	CheckMatrix =  np.mat('1,0,1,0,0;\
		               0,1,0,1,0;\
		               1,1,0,0,1')

	lookup ={ 0 : [],
		  1 : [4],
		  2 : [3],
		  4 : [2],
		  3 : [1],
		  5 : [0],
		  6 : [0,1],
		  7 : [1,2],
		 }

	for i in range(len(valid_lsb_list)):

		lsbVector = valid_lsb_list[i]
		# msg: list of 3 bits of secret image
		msgVector = msg[i]

		Syndrome = np.dot(lsbVector,CheckMatrix.transpose())

		# Sxor = syndrome XOR delta
		newSyndrome = np.mod(Syndrome + msgVector, 2)
		# Convert Sxor into decimal

		decSyndrome = toDec(np.asarray(newSyndrome),3)

		# Flip bits
		for bit in lookup[decSyndrome]:
			if(valid_lsb_list[i][bit]):
				valid_lsb_list[i][bit] -= 1
			else:
				valid_lsb_list[i][bit] += 1


	valid_lsb_list = [bit for vector in valid_lsb_list for bit in vector]

	lsb_list = valid_lsb_list + lsb_list[encodedBits:]





	list_index = 0
	ReferenceBlocks = GenerateBlocks(ReferenceImage,H,W)




	for i in range(len(PixelDifference)):

		for j in range(len(quantisation_table)):
			if PixelDifference[i] >= quantisation_table[j][0] and PixelDifference[i] <= quantisation_table[j][1]:
				break

		X = min(quantisation_table[j][1] - PixelDifference[i],PixelDifference[i] - quantisation_table[j][0])

		if X < 4:
			bits = 1
		else:
			bits = int(math.floor(math.log(X,2))) - 1

		lsb0 = lsb_list[list_index:list_index+bits]
		lsb0 = toDec(lsb0,bits)
		blocks[i][0] = blocks[i][0] - blocks[i][0] % 2**bits + lsb0
		# blocks[i][0] = OPAP(ReferenceBlocks[i][0],blocks[i][0],bits)

		list_index += bits

		lsb1 = lsb_list[list_index:list_index+bits]
		lsb1 = toDec(lsb1,bits)
		blocks[i][1] = blocks[i][1] - blocks[i][1] % 2**bits + lsb1
		# blocks[i][0] = OPAP(ReferenceBlocks[i][0],blocks[i][0],bits)

		list_index += bits




	CoverImage = ReconstructImage(blocks,H,W)

	new_img = Image.fromarray(CoverImage)
	#new_img.save("%s.bmp" %("stego_"+image))






	ReferenceBlocks = GenerateBlocks(ReferenceImage,H,W)
	RecoveredBlocks = GenerateBlocks(CoverImage,H,W)

	PixelDifference = [abs(int(ReferenceBlocks[i][1])-int(ReferenceBlocks[i][0])) for i in range(len(ReferenceBlocks))]

	lsb_list = []

	for i in range(len(PixelDifference)):

		for j in range(len(quantisation_table)):
			if PixelDifference[i] >= quantisation_table[j][0] and PixelDifference[i] <= quantisation_table[j][1]:
				break

		X = min(quantisation_table[j][1] - PixelDifference[i],PixelDifference[i] - quantisation_table[j][0])

		if X < 4:
			bits = 1
		else:
			bits = int(math.floor(math.log(X,2))) - 1

		lsb0 = (RecoveredBlocks[i][0] % (2**bits))
		lsb0 = toBinary(lsb0,bits)
		lsb_list += lsb0

		lsb1 = (RecoveredBlocks[i][1] % (2**bits))
		lsb1 = toBinary(lsb1,bits)
		lsb_list += lsb1



	valid_lsb_list = convert_lsb(lsb_list)

	msg_bits = valid_lsb_list

	RecoveredBits = np.mod(np.dot(msg_bits, CheckMatrix.transpose()),2)




	bin_msg_bits = RecoveredBits.reshape(1,-1).tolist()[0]
	output_msg = bin_to_str(bin_msg_bits)












	CoverImgTemp = CoverImage.astype(np.int16)
	ReferenceImgTemp = ReferenceImage.astype(np.int16)


	total_pixels = H*W

	change = CoverImgTemp - ReferenceImgTemp

	# DifferenceImage = CoverImage - ReferenceImage
	# diff_img = Image.fromarray(DifferenceImage  )
	# diff_img.save("%s.bmp" %("Difference_"+image))
	#
	# plt.imshow(diff_img,cmap='gray',vmin=0,vmax=255)
	# plt.title("Difference Image")
	# plt.show()

	mean_sqr_error = np.sum((change)**2)/float(total_pixels)
	PSNR = 10*np.log10(total_pixels/mean_sqr_error)
	embedding_rate = float(len_msg_bits)/total_pixels
	non_zero = np.count_nonzero(change)
	change_density = float(non_zero)*100/ total_pixels


	net_psnr += PSNR
	net_embed += embedding_rate
	net_change += change_density
	net_p += len_msg_bits

net_psnr /= limit
net_embed /= limit
net_change /= limit
net_p /= limit


# In[ ]:


print("{:25}".format("PSNR"), net_psnr)
print("{:25}".format("Embedding Rate"), net_embed)
print("{:25}".format("Change Density"),net_change)
print("{:25}".format("Capacity"),net_p)
