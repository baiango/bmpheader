import base64

# Replace 'output_file.txt' with the path to your base85-encoded file
input_file_path = 'output_file.txt'
# Replace 'decoded_file.bin' with the path where you want to save the decoded binary file
output_file_path = 'decoded_file.bin'

# Read the base85 encoded file
with open(input_file_path, 'rt') as text_file:
	encoded_data = text_file.read()

# Decode the data from base85
binary_data = base64.b85decode(encoded_data)

# Write the binary data to a file
with open(output_file_path, 'wb') as binary_file:
	binary_file.write(binary_data)

print(f'File {input_file_path} has been decoded from base85 and saved as {output_file_path}')
