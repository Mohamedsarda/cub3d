from PIL import Image
import os

# Define the path to the folder containing the images
folder_path = './Textures/png/guns_0/'

# Define the scale factor for resizing (e.g., 2 will double the size)
scale_factor = 1.5

# Loop through each image file
for i in range(181):
    # Construct the file name
    file_name = f"{i}.png"
    file_path = os.path.join(folder_path, file_name)

    # Check if the file exists
    if os.path.exists(file_path):
        # Open the image
        with Image.open(file_path) as img:
            # Calculate the new size
            new_size = (int(img.width * scale_factor), int(img.height * scale_factor))

            # Resize the image
            resized_img = img.resize(new_size, Image.LANCZOS)

            # Save the resized image, you can overwrite the original or save to a new file
            resized_img.save(file_path)
            print(f"Resized {file_name} to {new_size}")
    else:
        print(f"{file_name} does not exist")

print("Resizing completed.")
