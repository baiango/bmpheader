import base64

# Replace 'input_file.bin' with the path to your input binary file
input_file_path = 'input_file.bin'
# Replace 'output_file.txt' with the path to your output base85-encoded file
output_file_path = 'output_file.txt'

# Read the binary file
with open(input_file_path, 'rb') as binary_file:
	binary_data = binary_file.read()

# Encode the data to base85
encoded_data = base64.b85encode(binary_data)

# Write the base85 encoded data to a file
with open(output_file_path, 'wt') as text_file:
	text_file.write(encoded_data.decode('ascii'))  # base85 is ASCII text

print(f'File {input_file_path} has been encoded to base85 and saved as {output_file_path}')
